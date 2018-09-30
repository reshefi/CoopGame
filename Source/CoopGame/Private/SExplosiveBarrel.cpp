// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "SHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Controller.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultHealth = 60.0;
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnDamageTaken); //Registering to the event
	AreaOfDamage = CreateDefaultSubobject<USphereComponent>(TEXT("Radius Of impact"));
	AreaOfDamage->SetSphereRadius(200.0f);
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh"));
	BarrelMesh->SetupAttachment(RootComponent);
	AreaOfDamage->SetupAttachment(BarrelMesh);
	KineticRadialImpact = CreateDefaultSubobject<URadialForceComponent>("Kinetic Effect");
	ForceUp = 200000;
	//GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	//AreaOfDamage->SetupAttachment(BarrelMesh);

}

void ASExplosiveBarrel::OnDamageTaken(USHealthComponent * MyHealthComp, float Health, float HealthDelta,
	const  UDamageType* DamageType,  AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bExploded)
	{
		// Die !
		bExploded = true;
		// play particle effect...
		if (BlastEffect) {
			FRotator ActorRotator = GetActorRotation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlastEffect, GetActorLocation(), ActorRotator);
		}

		if (MaterialPostBlast) {
			BarrelMesh->SetMaterial(0, MaterialPostBlast);
		}
		//Now I push away nearby physics actors...
		
		if (KineticRadialImpact) {
			KineticRadialImpact->RegisterComponent();
			KineticRadialImpact->SetWorldLocation(GetActorLocation());
			KineticRadialImpact->SetWorldRotation(GetActorRotation());
			KineticRadialImpact->Radius = 1700;
			KineticRadialImpact->ForceStrength = 100000;
			KineticRadialImpact->ImpulseStrength = 100000;
			KineticRadialImpact->AttachTo(this->GetRootComponent(), NAME_None, EAttachLocation::KeepWorldPosition);
			KineticRadialImpact->FireImpulse();
		}

		//Now I lunch barrel upword...
		FVector  ImpulseVec;
		ImpulseVec.Set(0, 0, ForceUp);
		BarrelMesh->AddImpulse(ImpulseVec);
	}
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	bExploded = false;
}



