// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "SCharacter.h"
#include "TimerManager.h"



// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;

	bIsPowerupActive = false;

	SetReplicates(true);
}


void ASPowerupActor::OnTickPowerup(AActor * OtherActor)
{
	TicksProcessed++;

	OnPowerupTicked();
	if (TicksProcessed >= TotalNrOfTicks) {

		OnExpired(OtherActor);

		//Delete timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void ASPowerupActor::ActivatePowerup(AActor * OtherActor)
{
	OnActivated(OtherActor);
	bIsPowerupActive = true;
	OnRep_PowerupActive();
	if (PowerupInterval > 0.0f) {
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("OnTickPowerup"), OtherActor);
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, TimerDel, PowerupInterval, true);
	}
	else {
		OnTickPowerup(OtherActor);
	}
}


void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
	
}