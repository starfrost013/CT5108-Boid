//
// Boid Simulator
// Copyright © 2025 starfrost 
//

#include "BoidObject.h"
#include "BoidManager.h"

#define BOID_EPSILON_LENGTH			256.0f

// Sets default values
ABoidObject::ABoidObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a sphere
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidSphere"));
}

FVector ABoidObject::Steer(FVector goalPosition, BoidSteeringBehaviour steeringBehaviourType)
{
	FVector myLocation = GetActorLocation();
	FVector newCurrentVelocity; 

	switch (steeringBehaviourType)
	{
		case BoidSteeringBehaviour::Seek:
			newCurrentVelocity = goalPosition - myLocation; 
			break;
		case BoidSteeringBehaviour::Flee:
			newCurrentVelocity = myLocation - goalPosition;
			break;
		case BoidSteeringBehaviour::Pursue:
			// try to use it
			newCurrentVelocity = goalPosition; 
			break;
		case BoidSteeringBehaviour::Evade:
			newCurrentVelocity = -goalPosition;
			break;
	}
	newCurrentVelocity.Normalize();
	return newCurrentVelocity;
}

FVector ABoidObject::Flock(TArray<ABoidObject*> neighbours, BoidFlockingBehaviour flockingBehaviourType)
{
	FVector currentFlockVel = FVector(), averageLocation = FVector();
	uint32_t numNeighbours = neighbours.Num();

	if (!numNeighbours)
		return currentFlockVel;

	switch (flockingBehaviourType)
	{
		case BoidFlockingBehaviour::Alignment:
			for (ABoidObject* boid : neighbours)
				currentFlockVel += boid->currentVelocity;

			currentFlockVel /= numNeighbours;
			break;
		case BoidFlockingBehaviour::Cohere:
			for (ABoidObject* boid : neighbours)
				averageLocation += boid->GetActorLocation();

			averageLocation /= numNeighbours;

			currentFlockVel = Steer(averageLocation, BoidSteeringBehaviour::Seek);
			// additional cohere steps
			break;
		case BoidFlockingBehaviour::Separate:
			for (ABoidObject* boid : neighbours)
				currentFlockVel += Steer(boid->GetActorLocation(), BoidSteeringBehaviour::Flee);

			currentFlockVel /= numNeighbours;
			break;
	}
	currentFlockVel.Normalize();

	return currentFlockVel;
}


FVector ABoidObject::Wander()
{
	FVector position = GetActorLocation();

	double dist = (position - currentWanderTarget).Size();

	// the boid will spazz around constantly because if the new target is too close it may endlessly try to select new targets.
	// let's store our old wander target, and wait until we're more than the epsilon away from THAT before picking a new one (as long as we're not also right next to the new one)

	bool isFarEnoughAwayFromOld = ((position - oldWanderTarget).Size()) > BOID_EPSILON_LENGTH;

	// pick a random target
	if (currentWanderTarget == FVector::ZeroVector
	|| FMath::Abs(dist) < BOID_EPSILON_LENGTH
		&& isFarEnoughAwayFromOld)
	{
		oldWanderTarget = currentWanderTarget;
		currentWanderTarget = (position + (FMath::VRand() * manager->WanderRadius));

	}

	FVector wanderVelocity = Steer(currentWanderTarget, BoidSteeringBehaviour::Seek);
	return wanderVelocity;
}

// Squirrels don't have strong heads
FVector ABoidObject::Avoidance()
{
	float radius = manager->EyesightLength;

	// Let's hope 1.0 = 1 meter
	AActor* thingWeHit = manager->AnythingInTheWay(this, radius);
	FRotator finalRotation = targetVelocity.Rotation();
	FVector velocity = FVector::ZeroVector;

	// obstacle avoidance
	if (thingWeHit)
	{
		velocity = GetActorLocation() - thingWeHit->GetActorLocation();
		double dist = velocity.Size();

		// add an increasingly strong repulsive force as we get closer to the thing
		FVector finalVelocity = velocity * (1 / (dist / 64.0f));

		// ignore invalid avlues
		if (finalVelocity.ContainsNaN())
			finalVelocity = FVector::ZeroVector;

		return finalVelocity;

	}
	else
		return FVector::ZeroVector;
}

void ABoidObject::SetPhysicsType()
{
	if (manager->PhysicsType != manager->PHYSICS_TYPE_NONE)
	{
		mesh->SetSimulatePhysics(true);
		
		if (manager->PhysicsType == manager->PHYSICS_TYPE_PHYS_NOGRAV)
			mesh->SetEnableGravity(false);
	}
}

// Called when the game starts or when spawned
void ABoidObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoidObject::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Steer for this frame with deltatime applied

	targetVelocity = Flock(manager->GetBoidsWithinRange(this, manager->BoidPerceptionRadius), BoidFlockingBehaviour::Separate) * manager->SeparationWeight;
	targetVelocity += Flock(manager->GetBoidsWithinRange(this, manager->BoidPerceptionRadius), BoidFlockingBehaviour::Cohere) * manager->CohesionWeight;
	targetVelocity += Flock(manager->GetBoidsWithinRange(this, manager->BoidPerceptionRadius), BoidFlockingBehaviour::Alignment) * manager->AlignmentWeight;
	targetVelocity += Wander();

	// try and steer away from any obstacles e.g. other boids

	// reasonable rough estimate for the base size until we figure out what unreal is doing with it
	targetVelocity += Avoidance();
	//if (escapeVelocity != FVector::ZeroVector)
		//targetVelocity = escapeVelocity;

    targetVelocity.Normalize();
	targetVelocity *= manager->BaseSpeed;

	if (manager->PhysicsType == manager->PHYSICS_TYPE_NONE)
		SetActorLocation(GetActorLocation() + (targetVelocity) * deltaTime);
	else
		mesh->AddImpulse(targetVelocity);
	
	SetActorRotation(targetVelocity.Rotation());

	currentVelocity = targetVelocity;
}

