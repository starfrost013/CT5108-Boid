// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BoidObject.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class BOID_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();
	virtual void Tick(float DeltaTime) override;
	ABoidObject* FindNearestBoid(FVector position);
	// apply to target to get flock
	TArray<ABoidObject*> GetBoidsWithinRange(ABoidObject* boid, float range);

	// Simulation properties
	// These are not really directly used but are instead transferred into the actual simulation (NOTE: STDINT types cannot be used as UProperties!) 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int BoidCount;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BoidSpawnRadius;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")	
	float AlignmentWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float CohesionWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SeparationWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float FlockingBehaviourRadius;

	// 0 = No Physics
	// 1 = Physics no gravity
	// 2 = Physics w/Gravity (Doesn't make sense for this not to be simwide)
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int PhysicsType;

	constexpr int32_t PHYSICS_TYPE_NONE = 0;
	constexpr int32_t PHYSICS_TYPE_PHYS_NOGRAV = 1;
	constexpr int32_t PHYSICS_TYPE_PHYS_FULL = 2;


	USceneComponent* transform;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	struct BoidInitialisationData
	{
		uint32_t count;
		float radius; 
		TArray<ABoidObject*> boids;
	};

	ABoidObject* target;				// The current tagged boid
	BoidInitialisationData initData;
};
