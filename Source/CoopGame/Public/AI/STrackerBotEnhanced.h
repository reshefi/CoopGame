// Fill out your copyright notice in the Description page of Project Settings.
#define  DEFAULT_RANGE 100
#define  MAX_POWER_LEVEL 4
#pragma once

#include "CoreMinimal.h"
#include "AI/STrackerBot.h"
#include "STrackerBotEnhanced.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class COOPGAME_API ASTrackerBotEnhanced : public ASTrackerBot
{
	GENERATED_BODY()

		ASTrackerBotEnhanced();
public: 
	
	void BeginPlay();
protected:

	//Sphere where we will look for overlapped pawns.
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	USphereComponent * ZoneOfBotSearch;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	int32 NumberOfBotsClose;

	//Number of possible Trackers which can be in a scene and in the overlap zone
	int32 MaximumPower;

	//Number of current Trackers in this pawns overlap's zone.
	int32 PowerLevel;
	
	//Number of Maximum Bot Trackers that could be on a level
	int32 MaxPowerLevel;
	// The timer that sets the overlap check every second
	FTimerHandle TimerHandle_OverlapBotsCheck;

	float InitialExplosionDamage;
	//A function which is called once every second and recaculate the firepower and 
	UFUNCTION()
	void CalculateBlastPower();
};
