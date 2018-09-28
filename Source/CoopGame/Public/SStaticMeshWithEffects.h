// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SStaticMeshWithEffects.generated.h"

class UStaticMeshComponent;
class UCurveFloat;
class USIceEffectComponent;

UCLASS()
class COOPGAME_API ASStaticMeshWithEffects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASStaticMeshWithEffects();

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat * fCurve;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent * BasicMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USIceEffectComponent * matEffect;

	UFUNCTION()
	void PlayEffect(TArray<UMaterialInstanceDynamic *> target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
