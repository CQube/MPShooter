// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USHealthComponent;
class UMaterialInstanceDynamic;
class USoundCue;

UCLASS()
class MPSHOOTER_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	USHealthComponent* HealthComp;

	USphereComponent* SphereComp;

	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* HealthComponent, float Health, float HealthDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();
	
	//Next point in navigation path
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	// Material to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	void SelfDestruct();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	bool bExploded;

	bool bStartedSelfDestruction;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDamageInterval;
	
	FTimerHandle TimerHandle_SelfDamage;

	void DamageSelf();
	
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* SelfDestructSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* ExplodeSound;

	void OnCheckNearbyBots();

	/* power scale, boosted by buddies (range 1-4)*/
	int32 PowerLevel;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	FTimerHandle TimerHandle_RefreshPath;

	void RefreshPath();
};
