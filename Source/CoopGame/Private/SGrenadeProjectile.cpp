// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SGrenadeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/Pawn.h"
#include "Vector.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Array.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"



// Sets default values
ASGrenadeProjectile::ASGrenadeProjectile()
{

	GrenadeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeBody"));
	// Use a sphere as a simple collision representation
	DamageRadiusComp = CreateDefaultSubobject<USphereComponent>(TEXT("DamageRadius"));
	DamageRadiusComp->InitSphereRadius(5.0f);
	DamageRadiusComp->SetCollisionProfileName("Projectile");
	//DamageRadiusComp->OnComponentHit.AddDynamic(this, &ASGrenadeProjectile::OnHit);	// set up a notification for when this component hits something blocking

	RootComponent = GrenadeBodyMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = GrenadeBodyMesh;
	ProjectileMovement->InitialSpeed = 1600.f;
	ProjectileMovement->MaxSpeed = 2200;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 1 second by default
	InitialLifeSpan = 1.0f;
	OnDestroyed.AddDynamic(this, &ASGrenadeProjectile::WhenDestroyed);
	
}

void ASGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Grenade"));
	
}


void ASGrenadeProjectile::WhenDestroyed(AActor * act)
{
	UE_LOG(LogTemp, Warning, TEXT("I just Destroyed myself !"));
	// try and play the sound if specified
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(act, ExplosionSound, GetActorLocation());
	}

	if (ExplosionEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), true);
	}
	DrawDebugSphere(GetWorld(), GetActorLocation(), 32.0f, 12, FColor::Green, false, 10.0f);

	TArray<AActor*> OutOverlappingActors;
	DamageRadiusComp->GetOverlappingActors(OutOverlappingActors, UObject::StaticClass());
	FHitResult HitInfo;
	//OutOverlappingActors.Num();
	UE_LOG(LogTemp, Warning, TEXT("Actors overlap %s"), * DamageRadiusComp->GetReadableName());//OutOverlappingActors.Num());
	UE_LOG(LogTemp, Warning, TEXT("Actors overlap %d"), OutOverlappingActors.Num());
	//OutOverlappingActors
	for (auto actor : OutOverlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("Damage taken by %s"), *actor->GetName());
		UGameplayStatics::ApplyPointDamage(actor, 20.0f, actor->GetActorLocation(), HitInfo, nullptr, this, UDamageType::StaticClass());
	}
}

