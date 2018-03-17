// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityBase.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASAbilityBase::ASAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CooldownDuration = 0.1f;
	ImpactDuration = 0.1f;
	bIsOnCooldown = false;
	bIsCurrentlyExecuting = false;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASAbilityBase::SetupContext(ASCharacter* OwnerCharacter)
{
	AbilityOwner = OwnerCharacter;
	OnSetupContextFinished();
}


void ASAbilityBase::Activate()
{
	if (!bIsOnCooldown && !bIsCurrentlyExecuting)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_FinishImpact, this, &ASAbilityBase::AbilityExecutionFinished, ImpactDuration, false);
		bIsCurrentlyExecuting = true;
		
		if (Role < ROLE_Authority)
		{
			ServerExecuteAbility();
		}
		if(Role == ROLE_Authority)
			ExecuteAbility();
	}
}


void ASAbilityBase::ServerExecuteAbility_Implementation()
{
	ExecuteAbility();
}


bool ASAbilityBase::ServerExecuteAbility_Validate()
{
	return true;
}


void ASAbilityBase::AbilityExecutionFinished()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASAbilityBase::CooldownFinished, CooldownDuration, false);
	StopAbility();
	bIsOnCooldown = true;
	bIsCurrentlyExecuting = false;
}


void ASAbilityBase::CooldownFinished()
{
	bIsOnCooldown = false;
}


void ASAbilityBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASAbilityBase, AbilityOwner);
}