// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAbilityBase.generated.h"

class ASCharacter;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityBase")
	float CooldownDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityBase")
	float ImpactDuration;

	bool bIsOnCooldown;

	bool bIsCurrentlyExecuting;

	FTimerHandle TimerHandle_FinishImpact;

	FTimerHandle TimerHandle_Cooldown;

	UPROPERTY(BlueprintReadOnly, Category = "AbilityBase")
	ASCharacter* AbilityOwner;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void ExecuteAbility();

	UFUNCTION()
	void AbilityExecutionFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void StopAbility();

	UFUNCTION()
	void CooldownFinished();


public:

	void SetupContext(ASCharacter* OwnerCharacter);

	void Activate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnSetupContextFinished();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
	
