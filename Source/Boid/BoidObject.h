//
// Boid Simulator
// Copyright © 2025 starfrost 
//

#pragma once

#include "BoidBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidObject.generated.h"

class ABoidManager;

UCLASS()
class BOID_API ABoidObject : public ABoidBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidObject();


	struct BoidWeights
	{
		float alignment;
		float cohesion;
		float separation; 
	};

	virtual void Tick(float DeltaTime) override;

	// i am not sure this is a good ideas
	FVector Avoidance();


	bool target;											// The subject of the steering behaviour.
	//FVector velocity;
	BoidWeights weights; 
	float flockRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
