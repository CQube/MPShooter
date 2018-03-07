// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "../Public/Weapons/SWeapon.h"
#include "MPShooter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"


// Sets default values
ASWeapon::ASWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MuzzleSocketName = "MuzzleSocket";

	BaseDamage = 20.0f;
	BulletSpread = 2.0f;
	RateOfFire = 600;

	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency= 33.0f;
}


void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}


void ASWeapon::Fire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}
	OnFire();
}

void ASWeapon::OnFire()
{
	// implement in derived
}


void ASWeapon::ServerFire_Implementation()
{
	OnServerFire();
}


bool ASWeapon::ServerFire_Validate()
{
	return true;
}


void ASWeapon::OnServerFire()
{
	// implement in derived
}

void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}


void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}
