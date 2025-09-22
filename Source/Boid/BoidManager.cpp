// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	initData.count = 1000;
	initData.radius = 1024.0f;

	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(transform);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	for (uint32_t i = 0; i < initData.count; i++)
	{
		FVector spawnLocation = FMath::VRand() * initData.radius;
		FRotator spawnRotation = GetActorRotation();

		// see what this basic version does
		// the most efficient verion of this probably has these not as actors but as some sort of thing generartd by a shade ror something
		ABoidObject* object = GetWorld()->SpawnActor<ABoidObject>(spawnLocation, spawnRotation);

		// faster to statically allocate?
		initData.boids.Add(object);

	}
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto boid : initData.boids)
	{

	}


}

