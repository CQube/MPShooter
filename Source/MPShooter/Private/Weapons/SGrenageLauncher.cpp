// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenageLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "Animation/AnimInstance.h"

void ASGrenageLauncher::OnFire()
{
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
	
	// todo play the sound if specified
	// todo play a firing animation if specified
}

void ASGrenageLauncher::OnServerFire()
{
// 	if (GrenadeClass)
// 	{
// 		auto WeaponOwner = GetOwner();
// 
// 		if (WeaponOwner)
// 		{
// 			FVector EyeLocation;
// 			FRotator EyeRotation;
// 			WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
// 
// 			FVector ShotDirection = EyeRotation.Vector();
// 
// 			FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
// 			FRotator MuzzleRotation = MeshComponent->GetSocketRotation(MuzzleSocketName);
// 
// 			//Set Spawn Collision Handling Override
// 			FActorSpawnParameters ActorSpawnParams;
// 			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
// 			//ActorSpawnParams.Instigator = GetOwner();
// 
// 			// spawn the projectile at the muzzle
// 			GetWorld()->SpawnActor<AActor>(GrenadeClass, MuzzleLocation, EyeRotation, ActorSpawnParams);
// 		}
// 	}
}





