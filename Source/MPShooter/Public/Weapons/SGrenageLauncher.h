// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGrenageLauncher.generated.h"


/**
 * Class for launcher weapon
 */
UCLASS()
class MPSHOOTER_API ASGrenageLauncher : public ASWeapon
{
	GENERATED_BODY()
	
protected:

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	TSubclassOf<AActor> GrenadeClass;

public:

	virtual void OnFire() override;

	virtual void OnServerFire() override;

};
