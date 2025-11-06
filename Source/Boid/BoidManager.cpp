// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "BoidSuperKiller.h"
#include "PerformanceProfiler.h"

// Sets default values
ABoidManager::ABoidManager()
{
	GameProfiler_Init();
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(transform);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	initData.count_boids = BoidCount;
	initData.count_predators = PredatorCount;
	initData.radius = BoidSpawnRadius;

	FVector spawnLocation = FVector::ZeroVector;
	FRotator spawnRotation = FRotator::ZeroRotator;
	UWorld* world = GetWorld();


	for (uint32_t i = 0; i < initData.count_boids; i++)
	{
		spawnLocation = FMath::VRand() * initData.radius;
		//FRotator spawnRotation = GetActorRotation();
		spawnRotation = FMath::VRand().Rotation(); // doesn't make a lot of sense to use a static rotation

		// see what this basic version does
		// the most efficient verion of this probably has these not as actors but as some sort of thing generartd by a shade ror something
		ABoidObject* object = world->SpawnActor<ABoidObject>(BoidBlueprint, spawnLocation, spawnRotation);
		object->manager = this;
		object->weights.alignment = AlignmentWeight;
		object->weights.cohesion = CohesionWeight;
		object->weights.separation = SeparationWeight;

		// we're done so set the physics type
		object->SetPhysicsType();
	
		initData.boids.Add(object);
	}

	// spawn in the rotators
	for (uint32_t i = 0; i < initData.count_predators; i++)
	{
		spawnLocation = FMath::VRand() * initData.radius;
		//FRotator spawnRotation = GetActorRotation();
		spawnRotation = FMath::VRand().Rotation(); // doesn't make a lot of sense to use a static rotation

		ABoidSuperKiller* superKiller = world->SpawnActor<ABoidSuperKiller>(PredatorBlueprint, spawnLocation, spawnRotation);
		superKiller->manager = this;
	}
}

ABoidObject* ABoidManager::FindNearestBoid(FVector position)
{
	float currentMinimumDistance = 1e+30;
	ABoidObject* closestBoid = nullptr;

	for (auto boid : initData.boids)
	{
		// calculate the vector magnitude position
		// use absolute value as we do not care about direction currently
		float linearDistance = abs(FMath::Sqrt((position.X * position.X) + (position.Y * position.Y) + (position.Z * position.Z)));

		// ignore zero	
		if (linearDistance != 0.0f
			&& linearDistance < currentMinimumDistance)
		{
			currentMinimumDistance = linearDistance;
			closestBoid = boid;
		}
	}

	return closestBoid; 
}

TArray<ABoidObject*> ABoidManager::GetBoidsWithinRange(ABoidObject* boid, float range)
{
	// this is a terrible way of doing this
	TArray<ABoidObject*> neighbours;

	// don't bother iterating if there is nothing there
	if (range == 0.0f)
		return neighbours;

	for (auto currentBoid : initData.boids)
	{
		if (currentBoid == boid)
			continue;

		float distanceBetweenBoids = (currentBoid->GetActorLocation() - boid->GetActorLocation()).Size();

		if (distanceBetweenBoids < range)
			neighbours.Add(currentBoid);
	}

	return neighbours; 
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// See if anything is in the way of a boid object
// Fire a raycast in order to do this
AActor* ABoidManager::AnythingInTheWay(ABoidObject* boid, float lineLength)
{
	FHitResult hit;

	// calculate the start and end positions of the ray
	FVector startPosition = GetActorLocation();
	FVector endPosition = GetActorLocation() + lineLength;

	// don't hit ourselves
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hit, startPosition, endPosition, ECollisionChannel::ECC_Visibility, params))
		return hit.GetActor();
	else
		return nullptr;
}
