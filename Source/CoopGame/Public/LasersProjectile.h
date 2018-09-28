// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyIceEffectActor.h"
#include "LasersProjectile.generated.h"

class AMyIceEffectActor;
class UParticleSystem;
 
UCLASS(config = Game)
class ALasersProjectile : public AActor
{
	GENERATED_BODY()


public:
	ALasersProjectile();
		/** Sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Timeline")
		AMyIceEffectActor *  IceEffect;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UStaticMeshComponent* MainMaterialComp;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	TArray<USkeletalMeshComponent*> MainSkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class USoundBase* LaserExplosionSound;

	//Should be set by firing weapon ideally
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Weapon")
	UParticleSystem* MainImpactEffect;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:

	virtual void BeginPlay() override;
};

