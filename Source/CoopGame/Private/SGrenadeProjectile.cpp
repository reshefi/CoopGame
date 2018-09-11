// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SGrenadeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
ASGrenadeProjectile::ASGrenadeProjectile()
{

	GrenadeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeBody"));
	// Use a sphere as a simple collision representation
	DamageRadiusComp = CreateDefaultSubobject<USphereComponent>(TEXT("DamageRadius"));
	DamageRadiusComp->InitSphereRadius(5.0f);
	DamageRadiusComp->SetCollisionProfileName("Projectile");
	GrenadeBodyMesh->OnComponentHit.AddDynamic(this, &ASGrenadeProjectile::OnHit);	// set up a notification for when this component hits something blocking

	RootComponent = GrenadeBodyMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = GrenadeBodyMesh;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2500;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 1 second by default
	InitialLifeSpan = 1.0f;
}

void ASGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Grenade"));
	//HitComp
}


