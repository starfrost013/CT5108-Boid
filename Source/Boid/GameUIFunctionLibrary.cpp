//
// Boid Simulator
// Copyright © 2025 starfrost 
//


#include "GameUIFunctionLibrary.h"


// Bjarne Stroustrup should have never been allowed to touch a computer what is this :sob emoji here:
ABoidManager* UGameUIFunctionLibrary::boidManager;

void UGameUIFunctionLibrary::SetAlignment(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->AlignmentWeight = value * GAMEUI_SLIDER_LERP_BASE_SMALL; // slides are always from 0-1.
}

void UGameUIFunctionLibrary::SetCohesion(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->CohesionWeight = value * GAMEUI_SLIDER_LERP_BASE_SMALL; // slides are always from 0-1.
}

void UGameUIFunctionLibrary::SetSeparation(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->SeparationWeight = value * GAMEUI_SLIDER_LERP_BASE_SMALL; // slides are always from 0-1.
}

void UGameUIFunctionLibrary::SetWanderRadius(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->WanderRadius = value * GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

void UGameUIFunctionLibrary::SetFlockRadius(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->FlockingBehaviourRadius = value * GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

void UGameUIFunctionLibrary::SetBaseSpeed(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->BaseSpeed = value * GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

bool UGameUIFunctionLibrary::GetBoidManager(const UObject* worldContext)
{
	if (!boidManager)
	{
		// only do allocs if we need to
		TArray<AActor*> actors;

		UGameplayStatics::GetAllActorsOfClass(worldContext->GetWorld(), ABoidManager::StaticClass(), actors);

		if (actors.Num() == 0)
			return false; 

		// we don't really care about anything other than thef irst boidmanager, there should be only one
		boidManager = (ABoidManager*)actors[0];
		return true; 
	}

	return true;  
}