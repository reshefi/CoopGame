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
#include "UnrealNetwork.h"

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
	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetCollisionObjectType(ECC_PhysicsBody);
	AreaOfDamage->SetupAttachment(BarrelMesh);
	KineticRadialImpact = CreateDefaultSubobject<URadialForceComponent>("Kinetic Effect");
	ForceUp = 200000;
	SetReplicates(true);
	SetReplicateMovement(true);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	//AreaOfDamage->SetupAttachment(BarrelMesh);

}

void ASExplosiveBarrel::OnDamageTaken(USHealthComponent * OwningHealthComp, float Health, float HealthDelta,
	const  UDamageType* DamageType,  AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("My Health is: %f "), Health);
	UE_LOG(LogTemp, Warning, TEXT("Exploded is: %s"),  (bExploded ? TEXT("True") : TEXT("False")));
	if (Health <= 0.0f && !bExploded)
	{
		UE_LOG(LogTemp, Warning, TEXT("La La"));
		// play particle effect...
		MulticastExplode();
			//Now I push away nearby physics actors...
		if (KineticRadialImpact) {
			KineticRadialImpact->RegisterComponent();
			KineticRadialImpact->SetWorldLocation(GetActorLocation());
			KineticRadialImpact->SetWorldRotation(GetActorRotation());
			KineticRadialImpact->Radius = 700;
			KineticRadialImpact->ForceStrength = 1000;
			KineticRadialImpact->ImpulseStrength = 1000;
			KineticRadialImpact->bImpulseVelChange = true;
			KineticRadialImpact->bAutoActivate = false;
			KineticRadialImpact->bIgnoreOwningActor = true;
			KineticRadialImpact->AttachTo(this->GetRootComponent(), NAME_None, EAttachLocation::KeepWorldPosition);
			KineticRadialImpact->FireImpulse();
		}

		//Now I lunch barrel upword...
		FVector  ImpulseVec;
		ImpulseVec.Set(0, 0, ForceUp);
		BarrelMesh->AddImpulse(ImpulseVec);
	}
}

void ASExplosiveBarrel::Explode()
{
	// Die !
	/*bExploded = true;

	if (BlastEffect) {
		FRotator ActorRotator = GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlastEffect, GetActorLocation(), ActorRotator);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("BlastEffect null or not found"));
	}

	if (MaterialPostBlast) {
		BarrelMesh->SetMaterial(0, MaterialPostBlast);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MaterialPostBlast null or not found"));
	}*/
}


void ASExplosiveBarrel::MulticastExplode_Implementation()
{
	// Die !
	bExploded = true;
	if (Role == ROLE_Authority) {
		UE_LOG(LogTemp, Warning, TEXT("Authority"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Client"));
	}
	if (BlastEffect) {
		FRotator ActorRotator = GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlastEffect, GetActorLocation(), ActorRotator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BlastEffect null or not found"));
	}

	if (MaterialPostBlast) {
		BarrelMesh->SetMaterial(0, MaterialPostBlast);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MaterialPostBlast null or not found"));
	}
}

bool ASExplosiveBarrel::MulticastExplode_Validate()
{
	return true;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	bExploded = false;
}


void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASExplosiveBarrel, BlastEffect);
	DOREPLIFETIME(ASExplosiveBarrel, MaterialPostBlast);
	DOREPLIFETIME(ASExplosiveBarrel, bExploded);
	DOREPLIFETIME(ASExplosiveBarrel, BarrelMesh); 
}
