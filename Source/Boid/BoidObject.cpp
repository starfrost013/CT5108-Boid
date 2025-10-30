//
// Boid Simulator
// Copyright © 2025 starfrost 
//

#include "BoidObject.h"
#include "BoidManager.h"

// Sets default values
ABoidObject::ABoidObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a sphere
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidSphere"));
}

FVector ABoidObject::Steer(float deltaTime, FVector goalPosition)
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
			newCurrentVelocity = goalPosition; //TODO: make parameter
			break;
		case BoidSteeringBehaviour::Evade:
			newCurrentVelocity = -goalPosition;
			break;
	}


	return newCurrentVelocity;
}

FVector ABoidObject::Flock(float deltaTime, TArray<ABoidObject*> neighbours)
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

			steeringBehaviourType = BoidSteeringBehaviour::Seek;
			currentFlockVel = Steer(deltaTime, averageLocation);
			// additional cohere steps
			break;
		case BoidFlockingBehaviour::Separate:
			for (ABoidObject* boid : neighbours)
			{
				steeringBehaviourType = BoidSteeringBehaviour::Flee;
				currentFlockVel += Steer(deltaTime, boid->GetActorLocation());
			}

			currentFlockVel /= numNeighbours;
			break;
	}

	return currentFlockVel;
}

#define BOID_EPSILON_LENGTH			100.0f

FVector ABoidObject::Wander(float deltaTime)
{
	FVector position = GetActorLocation();
	double dist = (currentWanderTarget - GetActorLocation()).Size();

	// pick a random target
	if (currentWanderTarget == FVector::ZeroVector
	|| FMath::Abs(dist) < BOID_EPSILON_LENGTH)
	{
		currentWanderTarget = FMath::VRand() * manager->WanderRadius;
	}

	steeringBehaviourType = BoidSteeringBehaviour::Seek;
	return Steer(deltaTime, currentWanderTarget);
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

	flockingBehaviourType = BoidFlockingBehaviour::Separate;
	targetVelocity = Flock(deltaTime, manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius)) * manager->SeparationWeight;

	flockingBehaviourType = BoidFlockingBehaviour::Cohere;
	targetVelocity += Flock(deltaTime, manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius)) * manager->CohesionWeight;

	flockingBehaviourType = BoidFlockingBehaviour::Alignment;
	targetVelocity += Flock(deltaTime, manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius)) * manager->AlignmentWeight;

	targetVelocity += Wander(deltaTime);

	// try and steer away from any obstacles e.g. other boids

	// reasonable rough estimate for the base size until we figure out what unreal is doing with it
	float radius = 1024.0;

	// Let's hope 1.0 = 1 meter
	AActor* thingWeHit = manager->AnythingInTheWay(this, radius);

	if (thingWeHit)
	{
		// try and steer away
		FRotator rotation = GetActorRotation();

		// apply a turn
		FRotator away = thingWeHit->GetActorRotation() - rotation;

		SetActorRotation(rotation + ((away - rotation) * 0.01f)); //test
	}

	targetVelocity.Normalize();
	targetVelocity *= manager->BaseSpeed;

	if (manager->PhysicsType == manager->PHYSICS_TYPE_NONE)
		SetActorLocation(GetActorLocation() + (targetVelocity)*deltaTime);
	else
		mesh->AddImpulse(targetVelocity);

	SetActorRotation(targetVelocity.Rotation());

	currentVelocity = targetVelocity;
}

