// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveBarrel.h"
#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComponent;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	ExplosionRaduis = 200.0f;
	ExplosionDamage = 200.0f;
	ExplosionImpulse = 400;

	SetReplicates(true);
	SetReplicateMovement(true);
}


void AExplosiveBarrel::OnHealthChanged(USHealthComponent * HealthComponent, float Health, float HealthDelta, const UDamageType * DmgType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health <= 0.0f && !bExploded)
	{
		//Die
		bExploded = true;
		Explode();
	}
}


void AExplosiveBarrel::Explode()
{
	if (Role < ROLE_Authority)
	{
		ServerExplode();
	}
	// Draw Debug sphere at current position with ExplosionRaduis
	OnRep_Exploded();

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRaduis, 12, FColor::Red, false, 3.f);

	FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
	MeshComponent->AddImpulse(BoostIntensity, NAME_None, true);
	// Trigger animation


	RadialForceComp->FireImpulse();
	// Apply damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRaduis, DamageType, TArray<AActor*>(), this, GetInstigatorController());
}


void AExplosiveBarrel::OnRep_Exploded()
{
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	if (ExplodedMaterial)
	{
		MeshComponent->SetMaterial(0, ExplodedMaterial);
	}
}


void AExplosiveBarrel::ServerExplode_Implementation()
{
	Explode();
}


bool AExplosiveBarrel::ServerExplode_Validate()
{
	return true;
}


void AExplosiveBarrel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExplosiveBarrel, bExploded);
}