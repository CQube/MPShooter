// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class MPSHOOTER_API ASPowerupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	/* Time between powerup ticks */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval;

	/* Total times we apply the powerup effect */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNumberOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	// total number of ticks applied
	int32 TicksProcessed;

	UFUNCTION()
	void OnTickPowerup();


	/* Keeps power-up state */
	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool bIsPowerupActive;

	UFUNCTION()
	void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnPowerupStateChanged(bool bNewIsActive);

public:

	void ActivatePowerup(AActor * Activator);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnActivated(AActor * Activator);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnExpired();

};
