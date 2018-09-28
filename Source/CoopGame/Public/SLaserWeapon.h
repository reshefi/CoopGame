// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SLaserWeapon.generated.h"

/**
 * 
 */

class  UMaterialInstanceConstant;
class ALasersProjectile;

//UPROPERTY(BlueprintAssignable,   Category = "Material Transformation")
extern UMaterialInstanceConstant * IceMat;

UCLASS()
class COOPGAME_API ASLaserWeapon : public ASWeapon
{
	GENERATED_BODY()

public:

	ASLaserWeapon();

	UMaterialInstanceConstant * GetIceMat();

protected:
	virtual void Fire() override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ALasersProjectile> LaserProjectileClass;





	
		
	
};
