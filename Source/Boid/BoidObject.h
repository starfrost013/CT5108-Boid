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
	virtual void Tick(float DeltaTime) override;

	// i am not sure this is a good ideas
	void UpdateBoid(float deltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	UStaticMeshComponent* mesh;

	FVector2D velocity; 
};
