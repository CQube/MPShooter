// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGrenade.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class MPSHOOTER_API ASGrenade : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* GrenadeMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float TimeToExplode;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionRaduis;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION()
	void Explode();
public:	
	// Sets default values for this actor's properties
	ASGrenade();	
	
};
