// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidObject.h"

// Sets default values
ABoidObject::ABoidObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a sphere
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidSphere"));

	// get a basic unreal static mesh
	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.sphere'")).Object;
	mesh->SetStaticMesh(sphereMesh);
	mesh->SetSimulatePhysics(true);

	this->SetRootComponent(mesh);
}


void ABoidObject::UpdateBoid(float deltaTime, FVector targetPosition)
{
	// Initially, utilise
	// test

	FVector velocity;

	switch (steer)
	{
		case BoidSteeringBehaviour::Seek:
			velocity = targetPosition - GetActorLocation();
			break;	
		case BoidSteeringBehaviour::Flee:
			velocity = GetActorLocation() - targetPosition;
			break;
	}

	mesh->AddImpulse(velocity);
}

// Called when the game starts or when spawned
void ABoidObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

