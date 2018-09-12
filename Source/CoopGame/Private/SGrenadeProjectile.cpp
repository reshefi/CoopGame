// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SGrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"




// Sets default values
ASGrenadeProjectile::ASGrenadeProjectile()
{

	GrenadeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeBody"));
	// Use a sphere as a simple collision representation
	DamageRadiusComp = CreateDefaultSubobject<USphereComponent>(TEXT("DamageRadius"));
	DamageRadiusComp->InitSphereRadius(1000.0f);
	DamageRadiusComp->SetCollisionProfileName("Projectile");
	DamageRadiusComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageRadiusComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageRadiusComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DamageRadiusComp->SetHiddenInGame(false);
	GrenadeBodyMesh->SetSimulatePhysics(true);
	DamageRadiusComp->SetupAttachment(GrenadeBodyMesh);
	DamageRadiusComp->SetVisibility(true);
	RootComponent = GrenadeBodyMesh;
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = GrenadeBodyMesh;
	ProjectileMovement->InitialSpeed = 1100.f;
	ProjectileMovement->MaxSpeed = 2200;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	
	// Die after 1 second by default
	InitialLifeSpan = 1.0f;
	OnDestroyed.AddDynamic(this, &ASGrenadeProjectile::WhenDestroyed);
}

void ASGrenadeProjectile::WhenDestroyed(AActor * act)
{
	// try and play the sound if specified
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(act, ExplosionSound, GetActorLocation());
	}

	if (ExplosionEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), true);
	}
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 20.0f, GetActorLocation(),
		100.0f, UDamageType::StaticClass(), TArray<AActor*>(), this, (AController*)GetOwner(), false);
}

