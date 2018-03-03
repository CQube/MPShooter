// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "SGameMode.h"

static int32 EnableFriendlyFire = 0;
FAutoConsoleVariableRef CVAREnableFriendlyFire(
	TEXT("COOP.EnableFriendlyFire"),
	EnableFriendlyFire,
	TEXT("Enable friendly fire"),
	ECVF_Cheat);

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.0f;
	bIsDead = false;

	TeamID = 255;

	SetIsReplicated(true);
}

// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only hook if we are server
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* CompOwner = GetOwner();
		if (CompOwner)
		{
			CompOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
}


void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}


void USHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType,
	AController * InstigatedBy, AActor * DamageCauser)
{
	// if friendly fire is disabled => return on damaging friendly character
	if ((EnableFriendlyFire == 0 && IsFriendly(DamagedActor, DamageCauser)))
		return;

	if(Damage <= 0 || IsDead() && DamageCauser != DamagedActor)
		return;

	// Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}


bool USHealthComponent::IsDead()
{
	return bIsDead;
}


float USHealthComponent::GetHealth() const
{
	return Health;
}


void USHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0 || Health <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));
	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

bool USHealthComponent::IsFriendly(AActor * ThisActor, AActor * AnotherActor)
{
	if (ThisActor == nullptr || AnotherActor == nullptr) {
		// Assume Friendly
		return true;
	}
	USHealthComponent* HealthComponentThis = Cast<USHealthComponent>(ThisActor->GetComponentByClass(USHealthComponent::StaticClass()));
	USHealthComponent* HealthComponentAnother = Cast<USHealthComponent>(AnotherActor->GetComponentByClass(USHealthComponent::StaticClass()));

	if (HealthComponentThis == nullptr || HealthComponentAnother == nullptr) {
		// Assume Friendly
		return true;
	}

	return HealthComponentThis->TeamID == HealthComponentAnother->TeamID;
}


void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}
