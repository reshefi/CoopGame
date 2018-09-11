// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SGrenadeWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/SGrenadeProjectile.h"



// Called when the game starts or when spawned
void ASGrenadeWeapon::BeginPlay()
{
	Super::BeginPlay();

}


ASGrenadeWeapon::ASGrenadeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASGrenadeWeapon::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Grenade"));

	// try and fire a projectile
	if (GrenadeProjectileClass)
	{
		USkeletalMeshComponent * GunMeshComponent  = MeshComp;
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("MuzzleFlashSocket");
		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("MuzzleFlashSocket");

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		//ActorSpawnParams.Instigator = this;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<ASGrenadeProjectile>(GrenadeProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
}  
