// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LasersProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SLaserWeapon.h"
#include "UObjectBaseUtility.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Array.h"
#include "SStaticMeshWithEffects.h"
#include "SIceEffectComponent.h"
#include "MyIceEffectActor.h"
#include "Kismet/GameplayStatics.h"

ALasersProjectile::ALasersProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Constructor start !!"));
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	IceEffect = CreateDefaultSubobject<AMyIceEffectActor>(TEXT("IceEffect"));
	// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentHit.AddDynamic(this, &ALasersProjectile::OnHit);		
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	IceEffect = CreateDefaultSubobject<AMyIceEffectActor>(TEXT("Timeline"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	UE_LOG(LogTemp, Warning, TEXT("Projectile Constructor Finish !!"));
}

void ALasersProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	bool isStaticMesh = false;
	bool isSkeletalMesh = false;
	UStaticMeshComponent* targetComp = nullptr;
	
	UE_LOG(LogTemp, Warning, TEXT("EndProjectile"));
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogTemp, Warning, TEXT("EndProjectile2"));
		
		//Get the mesh components.
		
		TArray<UStaticMeshComponent*> MaterialComps;
		OtherActor->GetComponents(MaterialComps); //Currently works with only one Mesh
		if (MaterialComps.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("EndProjectile3"));
			//OtherActor->SetActorTickEnabled(true);
			MainMaterialComp = MaterialComps[0];
			IceEffect->SetMesh(MainMaterialComp);
			//IceEffect->BeginPlay();
			IceEffect->OnLaserBeamStaticMeshHit();
		}

		TArray<USkeletalMeshComponent*> SkeletalMaterialComps;
		OtherActor->GetComponents(SkeletalMaterialComps); //Currently works with only one Mesh
		if (SkeletalMaterialComps.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("EndProjectile4"));
			//OtherActor->SetActorTickEnabled(true);
			MainSkeletalMeshComp = SkeletalMaterialComps;
			IceEffect->AddSkeletalMesh(MainSkeletalMeshComp);
			//IceEffect->BeginPlay();
			IceEffect->OnLaserBeamSkeletalMeshHit();
		}
		// try and play the sound if specified
		if (LaserExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LaserExplosionSound, GetActorLocation());
		}

		if (MainImpactEffect) {
			
				UGameplayStatics::SpawnEmitterAtLocation(this->GetWorld(), MainImpactEffect, OtherActor->GetTransform());
		}
		 
	}
		Destroy();
}

void ALasersProjectile::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("EndProjectile5"));
	IceEffect->BeginPlay();
	Super::BeginPlay();
}
