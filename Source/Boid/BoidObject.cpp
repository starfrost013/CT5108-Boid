//
// Boid Simulator
// Copyright © 2025 starfrost 
//

#include "BoidObject.h"
#include "BoidBase.h"
#include "BoidManager.h"


// Sets default values
ABoidObject::ABoidObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a sphere
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidSphere"));
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

	targetVelocity = Super::Flock<ABoidObject>(manager->GetBoidsWithinRange(this, manager->BoidPerceptionRadius), BoidFlockingBehaviour::Separate) * manager->SeparationWeight;
	targetVelocity += Super::Flock<ABoidObject>(manager->GetBoidsWithinRange(this, manager->BoidPerceptionRadius), BoidFlockingBehaviour::Cohere) * manager->CohesionWeight;
	targetVelocity += Super::Flock<ABoidObject>(manager->GetBoidsWithinRange(this, manager->BoidPerceptionRadius), BoidFlockingBehaviour::Alignment) * manager->AlignmentWeight;
	targetVelocity += Super::Wander();

	// try and steer away from any obstacles e.g. other boids
	targetVelocity += Avoidance();
	
	// if we are being actively targeted try to run away 
	if (aggressor)
		targetVelocity += DontGetEaten() * manager->SelfPreservationWeight;

    targetVelocity.Normalize();
	targetVelocity *= manager->BaseSpeed;

	if (manager->PhysicsType == manager->PHYSICS_TYPE_NONE)
		SetActorLocation(GetActorLocation() + (targetVelocity) * deltaTime);
	else
		mesh->AddImpulse(targetVelocity);

	SetActorRotation(targetVelocity.Rotation());

	currentVelocity = targetVelocity;
}

FVector ABoidObject::DontGetEaten()
{
	FVector getAwayFrom = aggressor->GetVelocity();
	return Steer(getAwayFrom, BoidSteeringBehaviour::Evade);
}