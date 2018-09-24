// Fill out your copyright notice in the Description page of Project Settings.

#include "SStaticMeshWithEffects.h"
#include "SIceEffectComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "SIceEffectComponent.h"
#include <iostream>
#include <new.h>
#include "Components/TimelineComponent.h"

// Sets default values
ASStaticMeshWithEffects::ASStaticMeshWithEffects()
{
 	BasicMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	BasicMesh->SetupAttachment(RootComponent);
	matEffect = CreateDefaultSubobject<USIceEffectComponent>(TEXT("MyTimeLIne"));
	
}

// Called when the game starts or when spawned
void ASStaticMeshWithEffects::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("EndProjectile"));
	//UE_LOG(LogTemp, Warning, TEXT("matEffect %s"));
	/*matEffect = NewObject<USIceEffectComponent>();
	if (matEffect) {
		matEffect->SetFCurve(this->fCurve);
	}  */

}

void ASStaticMeshWithEffects::PlayEffect(TArray<UMaterialInstanceDynamic *> target)
{
	if (matEffect) {
		matEffect->PlayEffect(target);
	}
}




