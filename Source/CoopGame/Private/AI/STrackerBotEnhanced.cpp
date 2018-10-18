// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBotEnhanced.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Set.h"
#include "SubclassOf.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "STrackerBot.h"

ASTrackerBotEnhanced::ASTrackerBotEnhanced() {

	UE_LOG(LogTemp, Warning, TEXT("Working"));

	//Initializing range overlap with other bots
	ZoneOfBotSearch = CreateDefaultSubobject<USphereComponent>(TEXT("BotsSearch"));
	NumberOfBotsClose = DEFAULT_RANGE;
	ZoneOfBotSearch->SetSphereRadius(NumberOfBotsClose);
	ZoneOfBotSearch->SetupAttachment(RootComponent);
	MaxPowerLevel = MAX_POWER_LEVEL;
}

void ASTrackerBotEnhanced::BeginPlay() {
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_OverlapBotsCheck, this, &ASTrackerBotEnhanced::CalculateBlastPower, 1.0, true, 0.0);
	InitialExplosionDamage = ExplosionDamage;
	UE_LOG(LogTemp, Warning, TEXT("Initial explosion damage is: %d"), InitialExplosionDamage);
}

void ASTrackerBotEnhanced::CalculateBlastPower()
{
	UE_LOG(LogTemp, Warning, TEXT("Caculating Arms... "));
	TArray<AActor *> ActorSet;
	TArray<ASTrackerBotEnhanced *> BotSet;
	TSubclassOf<ASTrackerBot> PawnSubclass;
	int32 size = 0;
	ASTrackerBotEnhanced * Bot;
	ExplosionDamage =  InitialExplosionDamage;
	//UE_LOG(LogTemp, Warning, TEXT("Inflicted damage is: %f and initial damage is: %f"), ExplosionDamage, InitialExplosionDamage);
	ZoneOfBotSearch->GetOverlappingActors(ActorSet, PawnSubclass);
	for (int i = 0; i < ActorSet.Num(); ++i) {
		Bot = Cast<ASTrackerBotEnhanced>(ActorSet[i]);
		if (Bot && Bot != this) { //cast was successful
			size++;
		}
	}
	//int32 size = ActorSet.Num(); 
	PowerLevel = FMath::Clamp(size, 0 , MaxPowerLevel);  //MAX_POWER_LEVEL is number of bots
	ExplosionDamage += (PowerLevel) * 5;
	UE_LOG(LogTemp, Warning, TEXT("Number of TrackerBots: %d"), size);
	UE_LOG(LogTemp, Warning, TEXT("Alpha is: %f"), (PowerLevel / (float)MaxPowerLevel ));
	UE_LOG(LogTemp, Warning, TEXT("Inflicted damage is: %f and initial damage is: %f"), ExplosionDamage, InitialExplosionDamage);

	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	//It could still be null if nobody set a material in the first place
	if (MatInst)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing Material"));
		MatInst->SetScalarParameterValue("PowerLevelAlpha", (PowerLevel / (float)MaxPowerLevel));
	}
}



