//
// Boid Simulator
// Copyright © 2025 starfrost 
//


#include "GameUIFunctionLibrary.h"
#include "Util/GameVersion.h"


// Bjarne Stroustrup should have never been allowed to touch a computer what is this :sob emoji here:
ABoidManager* UGameUIFunctionLibrary::boidManager;

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

float UGameUIFunctionLibrary::GetAlignment(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return -1.0f;

	return boidManager->AlignmentWeight / GAMEUI_SLIDER_LERP_BASE_SMALL; // slides are always from 0-1.
}

float UGameUIFunctionLibrary::GetCohesion(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return -1.0f;

	return boidManager->CohesionWeight / GAMEUI_SLIDER_LERP_BASE_SMALL; // slides are always from 0-1.
}

float UGameUIFunctionLibrary::GetSeparation(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return -1.0f;

	return boidManager->SeparationWeight / GAMEUI_SLIDER_LERP_BASE_SMALL; // slides are always from 0-1.
}

float UGameUIFunctionLibrary::GetWanderRadius(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return -1.0f;

	return boidManager->WanderRadius / GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

float UGameUIFunctionLibrary::GetPerceptionRadius(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return -1.0f;

	return boidManager->BoidPerceptionRadius / GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

float UGameUIFunctionLibrary::GetBaseSpeed(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return -1.0f;

	return boidManager->BaseSpeed / GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

FString UGameUIFunctionLibrary::GetVersion()
{
	return FString(BOID_VERSION_STRING);
}

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

void UGameUIFunctionLibrary::SetPerceptionRadius(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->BoidPerceptionRadius = value * GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}

void UGameUIFunctionLibrary::SetBaseSpeed(const UObject* worldContext, float value)
{
	// don't explode if the world is not as we expect
	if (!boidManager && !GetBoidManager(worldContext))
		return;

	boidManager->BaseSpeed = value * GAMEUI_SLIDER_LERP_BASE_LARGE; // slides are always from 0-1.
}
