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
