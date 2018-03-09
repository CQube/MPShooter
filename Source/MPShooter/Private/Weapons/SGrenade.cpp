// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenade.h"
#include "../Public/Weapons/SGrenade.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASGrenade::ASGrenade()
{
	TimeToExplode = 3.0f;
	ExplosionRaduis = 50.0f;
	ExplosionDamage = 50.0f;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile"); // ok?

																			// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	GrenadeMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeMovementComp"));
	GrenadeMovementComp->UpdatedComponent = CollisionComp;
	GrenadeMovementComp->InitialSpeed = 3000.f;
	GrenadeMovementComp->MaxSpeed = 3000.f;
	GrenadeMovementComp->bRotationFollowsVelocity = true;
	GrenadeMovementComp->bShouldBounce = true;

	SetReplicates(true);
	SetReplicateMovement(true);
}


void ASGrenade::BeginPlay() {
	Super::BeginPlay();

	FTimerHandle TimerHandle_Explode;
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASGrenade::Explode, TimeToExplode);
}


void ASGrenade::Explode()
{
	// Draw Debug sphere at current position with ExplosionRaduis
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRaduis, 12, FColor::Red, false, 3.f);
	// Trigger animation
	if (ExplosionEffect)
	{
		MulticastExplodeFX();
	}
	// Apply damage

	if (Role == ROLE_Authority)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRaduis, DamageType, TArray<AActor*>(), this, GetInstigatorController());
	}
	// Self-destroy
	Destroy();
}

void ASGrenade::MulticastExplodeFX_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
}

bool ASGrenade::MulticastExplodeFX_Validate()
{
	return true;
}