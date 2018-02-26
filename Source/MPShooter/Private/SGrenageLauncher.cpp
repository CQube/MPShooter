// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenageLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "Animation/AnimInstance.h"

void ASGrenageLauncher::Fire()
{
	// try and fire a projectile
	if (GrenadeClass)
	{
		auto WeaponOwner = GetOwner();

		if (WeaponOwner)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();		
			
			
			FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotation = MeshComponent->GetSocketRotation(MuzzleSocketName);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//ActorSpawnParams.Instigator = GetOwner();

			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AActor>(GrenadeClass, MuzzleLocation, EyeRotation, ActorSpawnParams);
		}
	}

	// todo try and play the sound if specified
	// todo try and play a firing animation if specified
}



