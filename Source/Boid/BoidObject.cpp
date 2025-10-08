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
	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.sphere'")).Object;
	mesh->SetStaticMesh(sphereMesh);

	this->SetRootComponent(mesh);
}

void ABoidObject::Steer(float deltaTime, FVector startPosition)
{
	FVector targetLocation = startPosition;

	switch (steeringBehaviourType)
	{
		case BoidSteeringBehaviour::Seek:
			targetVelocity = targetLocation - startPosition;
			break;
		case BoidSteeringBehaviour::Flee:
			targetVelocity = startPosition - targetLocation;
			break;
		case BoidSteeringBehaviour::Pursue:
			// try to use it
			targetVelocity = startPosition; //TODO: make parameter
			break;
		case BoidSteeringBehaviour::Evade:
			targetVelocity = -startPosition;
			break;
	}

	targetVelocity.Normalize();
	targetVelocity *= deltaTime;

}

void ABoidObject::Flock(float deltaTime, FVector startPosition)
{
	switch (flockingBehaviourType)
	{
		case BoidFlockingBehaviour::Alignment:
			break;
		case BoidFlockingBehaviour::Cohere:
			break;
		case BoidFlockingBehaviour::Separate:
			break;
	}
}

void ABoidObject::UpdateBoid(float deltaTime, ABoidObject* targetObj)
{
	// Initially, utilise
	// test

	// Steer for this frame with deltatime applied
	if (target)
		steeringBehaviourType = BoidSteeringBehaviour::Seek;
	else
		steeringBehaviourType = BoidSteeringBehaviour::Flee;
	
	flockingBehaviourType = BoidFlockingBehaviour::Alignment;
	Flock(deltaTime, targetObj->GetActorLocation());
	flockingBehaviourType = BoidFlockingBehaviour::Cohere;
	Flock(deltaTime, targetObj->GetActorLocation());
	flockingBehaviourType = BoidFlockingBehaviour::Separate;
	Flock(deltaTime, targetObj->GetActorLocation());

	if (manager->PhysicsType == manager->PHYSICS_TYPE_NONE)
		SetActorLocation(GetActorLocation() + targetVelocity); // todo: split targetVelocity
	else
		mesh->AddImpulse(targetVelocity);

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

