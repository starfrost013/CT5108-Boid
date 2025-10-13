// Fill out your copyright notice in the Description page of Project Settings.

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
		Seek = 0,
		Flee = 1,
		Pursue = 2,
		Evade = 3,
		Arrive = 4,
		AvoidObstacles = 5,
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
	void UpdateBoid(float deltaTime, ABoidObject* targetObj);
	FVector Steer(float deltaTime, FVector startPosition);
	FVector Flock(float deltaTime, TArray<ABoidObject*> neighbours);
	void Wander(float deltaTime, FVector wanderRange);
	void SetPhysicsType();


	bool target;											// The subject of the steering behaviour.
	//FVector velocity;
	BoidFlockingBehaviour flockingBehaviourType;
	BoidSteeringBehaviour steeringBehaviourType;
	BoidWeights weights; 
	float flockRange;

	ABoidManager* manager;									// Set during spawn

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* mesh;
	FVector currentVelocity;
	FVector targetVelocity;

};
