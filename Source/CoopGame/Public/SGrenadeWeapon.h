// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGrenadeWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class ASGrenadeProjectile;
/**
 * 
 */
UCLASS()
class COOPGAME_API ASGrenadeWeapon : public ASWeapon
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ASGrenadeWeapon();
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire() override;
	
	virtual void BeginPlay() override;

public:

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ASGrenadeProjectile> GrenadeProjectileClass;
};
