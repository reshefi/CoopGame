// Fill out your copyright notice in the Description page of Project Settings.

#include "SLaserWeapon.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/Material.h"
#include "CollisionQueryParams.h"
#include "LasersProjectile.h"
#include "CoopGame.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"



UMaterialInstanceConstant * IceMat;

int init_IceMat()
{
	UE_LOG(LogTemp, Warning, TEXT("init_IceMat"));
	if (!IceMat) {
		IceMat = LoadObject<UMaterialInstanceConstant>(nullptr, TEXT("/Game/Ice/M_Ice_Inst"));
	}
	return IceMat != NULL;
}


ASLaserWeapon::ASLaserWeapon()
{
	init_IceMat();
}

UMaterialInstanceConstant * ASLaserWeapon::GetIceMat()
{
	return IceMat;
}

void ASLaserWeapon::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing"));
	UE_LOG(LogTemp, Warning, TEXT("Firing %s"), (IceMat != nullptr ?  TEXT("True") : TEXT("False")));

	//Trace the world, from pawn eyes to crosshair location

	AActor * MyOwner = GetOwner();
	if (MyOwner)
	{
		
		// try and fire a projectile
		if (LaserProjectileClass)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();
			FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			QueryParams.bReturnPhysicalMaterial = true;

			FVector TracerEndPoint = TraceEnd;

			USkeletalMeshComponent * GunMeshComponent = MeshComp;
			FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("MuzzleSocket");
			FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("MuzzleSocket");

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//ActorSpawnParams.Instigator = this;

			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<ALasersProjectile>(LaserProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		}

		
		

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

