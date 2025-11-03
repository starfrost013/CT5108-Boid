

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BoidManager.h"

#include "GameUIFunctionLibrary.generated.h"


/**
 * This part implements some nice C++ functions because I hate blueprints and it is harder than C++ IMO
 */
UCLASS()
class BOID_API UGameUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

// Blueprint function libraries must be static!
public:

	// Is it possible to set this as a UProperty e.g. in GameUI Blueprint?
	#define GAMEUI_SLIDER_LERP_BASE_SMALL	16			// for small stuff e.g. separation
	#define GAMEUI_SLIDER_LERP_BASE_LARGE	2048		// for large stuff

	// this is terrible
	static ABoidManager* boidManager;
	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Alignment", meta = (WorldContext = "WorldContext"))
	static float GetAlignment(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Cohesion", meta = (WorldContext = "WorldContext"))
	static float GetCohesion(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Separation", meta = (WorldContext = "WorldContext"))
	static float GetSeparation(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Wander Radius", meta = (WorldContext = "WorldContext"))
	static float GetWanderRadius(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Perception Radius", meta = (WorldContext = "WorldContext"))
	static float GetPerceptionRadius(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Base Speed", meta = (WorldContext = "WorldContext"))
	static float GetBaseSpeed(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Get Boid Simulator Version", meta = (WorldContext = "WorldContext"))
	static FString GetVersion();

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Set Boid Alignment", meta = (WorldContext = "WorldContext"))
	static void SetAlignment(const UObject* worldContext, float value);
	
	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Set Boid Cohesion", meta = (WorldContext = "WorldContext"))
	static void SetCohesion(const UObject* worldContext, float value);
	
	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Set Boid Separation", meta = (WorldContext = "WorldContext"))
	static void SetSeparation(const UObject* worldContext, float value);
	
	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Set Boid Wander Radius", meta = (WorldContext = "WorldContext"))
	static void SetWanderRadius(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Set Boid Perception Radius", meta = (WorldContext = "WorldContext"))
	static void SetPerceptionRadius(const UObject* worldContext, float value);

	UFUNCTION(BlueprintCallable, Category = "BoidManager Blueprint Library", DisplayName = "Set Boid Base Speed", meta = (WorldContext = "WorldContext"))
	static void SetBaseSpeed(const UObject* worldContext, float value);


private:
	static bool GetBoidManager(const UObject* worldContext);

};
