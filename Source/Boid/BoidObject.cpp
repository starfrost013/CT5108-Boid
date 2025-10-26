// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidManager.h"
#include "BoidObject.h"

// Sets default values
ABoidObject::ABoidObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a sphere
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidSphere"));

	// get a basic unreal static mesh
	/*UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Content/Models/squirrel/StaticMeshes/Squirrel.uasset'")).Object;
	mesh->SetStaticMesh(sphereMesh);

	this->SetRootComponent(mesh);*/
}

FVector ABoidObject::Steer(float deltaTime, FVector startPosition)
{
	FVector myLocation = GetActorLocation();
	FVector newCurrentVelocity; 

	switch (steeringBehaviourType)
	{
		case BoidSteeringBehaviour::Seek:
			newCurrentVelocity = myLocation - startPosition;
			break;
		case BoidSteeringBehaviour::Flee:
			newCurrentVelocity = startPosition - myLocation;
			break;
		case BoidSteeringBehaviour::Pursue:
			// try to use it
			newCurrentVelocity = startPosition; //TODO: make parameter
			break;
		case BoidSteeringBehaviour::Evade:
			newCurrentVelocity = -startPosition;
			break;
	}

	newCurrentVelocity.Normalize();
	newCurrentVelocity *= deltaTime;

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

	currentFlockVel.Normalize();
	return currentFlockVel;
}

void ABoidObject::UpdateBoid(float deltaTime, ABoidObject* targetObj)
{
	// Initially, utilise
	// test

	// Steer for this frame with deltatime applied

	flockingBehaviourType = BoidFlockingBehaviour::Separate;
	targetVelocity = Flock(deltaTime, manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius)) * manager->SeparationWeight;

	flockingBehaviourType = BoidFlockingBehaviour::Cohere;
	targetVelocity += Flock(deltaTime, manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius)) * manager->CohesionWeight;

	flockingBehaviourType = BoidFlockingBehaviour::Alignment;
	targetVelocity += Flock(deltaTime, manager->GetBoidsWithinRange(this, manager->FlockingBehaviourRadius)) * manager->AlignmentWeight;

	targetVelocity += Wander(deltaTime);

	targetVelocity *= manager->BaseSpeed; 

	if (manager->PhysicsType == manager->PHYSICS_TYPE_NONE)
		SetActorLocation(GetActorLocation() + (targetVelocity) * deltaTime); 
	else
		mesh->AddImpulse(targetVelocity);

	SetActorRotation(targetVelocity.Rotation());

	currentVelocity = targetVelocity; 
}

#define BOID_EPSILON_LENGTH			100.0f

FVector ABoidObject::Wander(float deltaTime)
{
	// pick a random target
	if (currentWanderTarget == FVector::ZeroVector
	|| (currentWanderTarget - GetActorLocation()).Size() < BOID_EPSILON_LENGTH)
	{
		currentWanderTarget = FMath::VRand() * wanderRadius;
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
void ABoidObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

