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

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UClass* BoidBlueprint;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UClass* PredatorBlueprint;

	// Simulation properties
	// These are not really directly used but are instead transferred into the actual simulation (NOTE: STDINT types cannot be used as UProperties!) 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int BoidCount;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int PredatorCount;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (UIMin = 0.0f, UIMax = 2.0f))
	float PredatorAggression;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BoidSpawnRadius;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")	
	float AlignmentWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float CohesionWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SeparationWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SelfPreservationWeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BoidPerceptionRadius;

	// 0 = No Physics
	// 1 = Physics no gravity
	// 2 = Physics w/Gravity (Doesn't make sense for this not to be sim-wide)
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int PhysicsType;

	// better than defines
	const int32_t PHYSICS_TYPE_NONE = 0;
	const int32_t PHYSICS_TYPE_PHYS_NOGRAV = 1;
	const int32_t PHYSICS_TYPE_PHYS_FULL = 2;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float WanderRadius;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float BaseSpeed;

	// The radius of the area the boid can see
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float EyesightLength;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float PredatorSightRadius;

	// The boid obliteration epsilon
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
