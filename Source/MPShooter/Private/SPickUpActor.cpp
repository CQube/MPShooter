// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickUpActor.h"
#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "SCharacter.h"

// Sets default values
ASPickUpActor::ASPickUpActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64.0f, 75.0f, 75.0f);
	DecalComp->SetupAttachment(RootComponent);

	CooldownDuration = 10.0f;
}

// Called when the game starts or when spawned
void ASPickUpActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}


void ASPickUpActor::Respawn()
{
	if (Role == ROLE_Authority)
	{
		if (!PowerupClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. Please update your Blueprint"), *GetName());
			return;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		PowerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
	}
}


void ASPickUpActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ASCharacter * PawnActor = Cast<ASCharacter>(OtherActor);
	if (PawnActor != nullptr && PawnActor->IsPlayerControlled())
	{
		//Todo: Grant a powerUp to player
		if (PowerupInstance && Role == ROLE_Authority)
		{
			PowerupInstance->ActivatePowerup(OtherActor);
			PowerupInstance = nullptr;

			GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickUpActor::Respawn, CooldownDuration);
		}
	}
}
