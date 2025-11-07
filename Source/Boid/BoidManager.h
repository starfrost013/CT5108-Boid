//
// Boid Simulator
// Copyright © 2025 starfrost 
//


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
	// Simulation properties
	// These are not really directly used but are instead transferred into the actual simulation (NOTE: STDINT types cannot be used as UProperties!) 
	// 
	// The blueprint containing the model to use for the regular boids.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UClass* BoidBlueprint;

	// The blueprint containing the model to use for the predator boids.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UClass* PredatorBlueprint;

	// THe number of boids to spawn.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int BoidCount;

	// The number of predators to spawn.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int PredatorCount;

	// The aggression level of the predators - controls their speed as well as how long itt akes them to pick a target
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (UIMin = 0.0f, UIMax = 2.0f))
	float PredatorAggression;

	// The spawn radius within which the boids will spawn.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BoidSpawnRadius;

	// Weight multiplier applied for the alignment flocking behaviour.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")	
	float AlignmentWeight;

	// Weight multiplier applied for the cohesion flocking behaviour.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float CohesionWeight;

	// Weight multiplier applied for the separation flocking behaviour.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SeparationWeight;

	// Weight multiplier applied for the self-preservation behaviou (how much to flee frrom a predatorr.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SelfPreservationWeight;

	// The eyesight radius of the boid.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BoidPerceptionRadius;

	/* 
	The type of physics behaviour to use.

	0 = No Physics
	1 = Physics no gravity
	2 = Physics w/Gravity (Doesn't make sense for this not to be sim-wide) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int PhysicsType;

	// better than defines
	const int32_t PHYSICS_TYPE_NONE = 0;
	const int32_t PHYSICS_TYPE_PHYS_NOGRAV = 1;
	const int32_t PHYSICS_TYPE_PHYS_FULL = 2;

	// The radius within which a new wander type will be selected.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float WanderRadius;

	// Base speed multiplier for all boid types.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BaseSpeed;

	// The sight radius for the predators.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float PredatorSightRadius;

	// The boid obliteration epsilon i.e. how close does a predator have to get to a squirrel before it gets aten
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float PredatorAttackRadius;

	// The maximum distance the predator will be able to see
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float PredatorMaxPursueDistance;

	// Sets default values for this actor's properties
	ABoidManager();

	USceneComponent* transform;

	virtual void Tick(float DeltaTime) override;
	ABoidObject* FindNearestBoid(FVector position);
	// apply to target to get flock
	TArray<ABoidObject*> GetBoidsWithinRange(ABoidObject* boid, float range);
	void RemoveBoid(ABoidObject* boid);

	// Return the first object that we hit
	AActor* AnythingInTheWay(ABoidObject* boid, float lineLength);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	struct BoidInitialisationData
	{
		uint32_t count_boids;
		uint32_t count_predators;
		float radius; 
		TArray<ABoidObject*> boids;
	};

	ABoidObject* target;				// The current tagged boid
	BoidInitialisationData initData;
};
