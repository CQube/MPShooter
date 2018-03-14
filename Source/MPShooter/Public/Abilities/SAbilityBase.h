// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAbilityBase.generated.h"

UCLASS()
class MPSHOOTER_API ASAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityBase")
	float CooldownDuration;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityBase")
	float ImpactDuration;

	bool bIsOnCooldown;

	bool bIsCurrentlyExecuting;

	FTimerHandle TimerHandle_FinishImpact;

	FTimerHandle TimerHandle_Cooldown;

	ACharacter* AbilityOwner;

protected:

	virtual void ExecuteAbility();

	UFUNCTION()
	void AbilityExecutionFinished();

	virtual void StopAbility();

	UFUNCTION()
	void CooldownFinished();


public:

	void SetupContext(ACharacter* OwnerCharacter);

	void Activate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
	
