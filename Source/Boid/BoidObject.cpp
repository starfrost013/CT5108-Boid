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

	targetVelocity = Flock(manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius), BoidFlockingBehaviour::Separate) * manager->SeparationWeight;
	targetVelocity += Flock(manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius), BoidFlockingBehaviour::Cohere) * manager->CohesionWeight;
	targetVelocity += Flock(manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius), BoidFlockingBehaviour::Alignment) * manager->AlignmentWeight;
	targetVelocity += Wander();

	// try and steer away from any obstacles e.g. other boids

	// reasonable rough estimate for the base size until we figure out what unreal is doing with it
	float radius = 2048.0;

	// Let's hope 1.0 = 1 meter
	AActor* thingWeHit = manager->AnythingInTheWay(this, radius);
	FRotator finalRotation = targetVelocity.Rotation();

	// obstacle avoidance
	if (thingWeHit)
	{
		// based on the distance away from it apply a different fading factor
	//	targetVelocity += Steer(deltaTime, thingWeHit->GetActorLocation(), BoidSteeringBehaviour::Flee) * manager->SeparationWeight * 2.0f;
	}

    targetVelocity.Normalize();
	targetVelocity *= manager->BaseSpeed;

	if (manager->PhysicsType == manager->PHYSICS_TYPE_NONE)
		SetActorLocation(GetActorLocation() + (targetVelocity) * deltaTime);
	else
		mesh->AddImpulse(targetVelocity);

	SetActorRotation(finalRotation);

	currentVelocity = targetVelocity;
}

