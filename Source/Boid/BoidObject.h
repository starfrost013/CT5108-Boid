//
// Boid Simulator
// Copyright © 2025 starfrost 
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidObject.generated.h"

class ABoidManager;

UCLASS()
class BOID_API ABoidObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidObject();

	enum BoidSteeringBehaviour
	{
		Seek = 0,							// Seek to a position.
		Flee = 1,							// Flee from a position.
		Pursue = 2,							// Pursue a position.
		Evade = 3,							// Evade (i.e. anticipate and try to get away from) a position.
		Arrive = 4,							// Try to find a position.
		SteerToAvoid = 5,					// Steer away from something.
		AvoidObstacles = 6,					// Avoid any obstacles.
	};

	enum BoidFlockingBehaviour
	{
		Alignment = 0,
		Cohere = 1,
		Separate = 2,
	};

	struct BoidWeights
	{
		float alignment;
		float cohesion;
		float separation; 
	};

	virtual void Tick(float DeltaTime) override;

	// i am not sure this is a good ideas
	FVector Steer(FVector startPosition, BoidSteeringBehaviour steeringBehaviourType);
	FVector Flock(TArray<ABoidObject*> neighbours, BoidFlockingBehaviour flockingBehaviourType);
	FVector Wander();
	FVector Avoidance();
	void SetPhysicsType();


	bool target;											// The subject of the steering behaviour.
	//FVector velocity;
	BoidWeights weights; 
	float flockRange;

	ABoidManager* manager;									// Set during spawn

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* mesh;
	FVector currentVelocity = FVector::ZeroVector;
	FVector targetVelocity = FVector::ZeroVector;
	FVector oldWanderTarget = FVector::ZeroVector;
	FVector currentWanderTarget = FVector::ZeroVector;

};
