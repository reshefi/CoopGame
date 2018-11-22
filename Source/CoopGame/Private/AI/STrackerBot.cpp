// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationPath.h"
#include "AI/Navigation/NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Array.h"
#include "SCharacter.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"

static int32 DebugTrackerBotDrawing = 0;

FAutoConsoleVariableRef CVARDebugTrackerBotDrawing(
	TEXT("COOP.DebugTrackerBot"),
	DebugTrackerBotDrawing,
	TEXT("Draw Debug Lines for TrackerBots"),
	ECVF_Cheat);

// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDamageComp"));
	ExplosionRadius = 350;
	SphereComp->SetSphereRadius(ExplosionRadius);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	bUseVelocityChange = true;
	MovementForce = 1500;
	RequiredDistanceToTarget = 100;
	bExploded = false;

	ExplosionDamage = 40;

	SelfDamageInterval = 0.25f;
	IsFirstPointTouched = false;
	SetReplicates(true);

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		//Find initial move to.
		NextPathPoint = GetNextPathPoint();
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	//Finding the best actor to attack
	AActor * BestTarget = nullptr;
	float NearestTargetDistance = FLT_MAX;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || USHealthComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		USHealthComponent* TestPawnHealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
		if (TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0.0f)
		{
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();

			if (Distance < NearestTargetDistance) 
			{
				BestTarget = TestPawn;
				NearestTargetDistance = Distance;
			}
		}
	}

	if (BestTarget)
	{
		UNavigationPath* NavPath = UNavigationSystem::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);
		GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
		GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ASTrackerBot::RefreshPath, 5.0f, false, 0.0f);
		if (NavPath && NavPath->PathPoints.Num() > 1) {
			// Return next point in the path
			return NavPath->PathPoints[1];
		}
	}
	// Failed to find path
	return GetActorLocation();
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent * OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	// @TODO: Pulse the material on hit

	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	//It could still be null if nobody set a material in the first place
	if (MatInst)
	{
		
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());

	// Explode on hitpoints == 0
	if (Health <= 0.0f) {
		SelfDestruct();
	}
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}

	bExploded = true;

	FRotator ActorRotator = GetActorRotation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), ActorRotator);

	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());

	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);  //Mesh will get destroyed at end of function.

	if (Role == ROLE_Authority)
	{
		TArray<AActor *> IgnoredActors;
		IgnoredActors.Add(this);

		// Apply Damage !
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		if (DebugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		}
		
		//Delete Actor immediately
		//Destroy();
		SetLifeSpan(2.0f);
	}
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = FGenericPlatformMath::Abs((GetActorLocation() - NextPathPoint).Size());

		if (DistanceToTarget <= RequiredDistanceToTarget) {
			NextPathPoint = GetNextPathPoint();
			if (DebugTrackerBotDrawing)
			{
				DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
			}
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("Distance to target is: %f"), DistanceToTarget);
			// Keep moving towards next target
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			FVector ForceCorrection = FVector::ZeroVector;
			ForceDirection.Normalize();
			FVector Velocity = MeshComp->GetPhysicsLinearVelocity();
			Velocity.Normalize();
			
			if (DistanceToTarget > 50)
			{
				float VelocityX = Velocity.GetComponentForAxis(EAxis::X);
				float VelocityY = Velocity.GetComponentForAxis(EAxis::Y);
				float ForceX = ForceDirection.GetComponentForAxis(EAxis::X);
				float ForceY = ForceDirection.GetComponentForAxis(EAxis::Y);
				float NewVelocityX = 0;
				float NewVelocityY = 0;
				bool changeOccured = false;
				float DeltaX = FGenericPlatformMath::Abs(VelocityX - ForceX);
				float DeltaY = FGenericPlatformMath::Abs(VelocityY - ForceY);

				if (FGenericPlatformMath::Abs(VelocityX - ForceX) > 1.05)
				{
					NewVelocityX = VelocityX * -1;
					NewVelocityY = VelocityY * -1;
					changeOccured = true;
				}
				else {
					NewVelocityX = VelocityX;
				}
				if (!changeOccured) 
				{
					if (FGenericPlatformMath::Abs(VelocityY - ForceY) > 0.95)
					{
						NewVelocityX = VelocityX * -1;
						NewVelocityY = VelocityY * -1;
						changeOccured = true;
					}
					else {
						NewVelocityY = VelocityY;
					}
				}
				if (changeOccured) { 
					ForceCorrection.Set(NewVelocityX, NewVelocityY, Velocity.GetComponentForAxis(EAxis::Z));
					ForceCorrection *= (MovementForce  * 5); 
					MeshComp->AddForce(ForceCorrection, NAME_None, bUseVelocityChange);
					//DrawDebugString(GetWorld(), MeshComp->GetComponentLocation(), FString::Printf(TEXT("DeltaX is %f\nDeltaY is %f"), DeltaX, DeltaY), this, FColor::Red,0);
					if (DebugTrackerBotDrawing) {
						DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceCorrection, 32, FColor::Blue, false, 0.0f, 0, 1.0f);
					}
				}
			}
			ForceDirection *= MovementForce;
			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
			//Note lifetime = 0 because we are in a Tick function.
			if (DebugTrackerBotDrawing) {
				DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
			}
		}
		if (DebugTrackerBotDrawing) {
			DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
		}
	}
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!bStartedSelfDestruction && !bExploded) {
		ASCharacter * PlayerPawn = Cast<ASCharacter>(OtherActor);
		if (PlayerPawn && !USHealthComponent::IsFriendly(OtherActor, this))
		{
			// We overlap with a player
			if (Role == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);
			}

			bStartedSelfDestruction = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}
