// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidObject.generated.h"

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
		Path = 0,
		Wall = 1,
		Other = 2,
	};

	virtual void Tick(float DeltaTime) override;

	// i am not sure this is a good ideas
	void UpdateBoid(float deltaTime, FVector targetPosition);

	bool target;											// The subject of the steering behaviour.
	//FVector velocity;
	BoidFlockingBehaviour flock;
	BoidSteeringBehaviour steer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:

	UStaticMeshComponent* mesh;

};
