// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "MyIceEffectActor.generated.h"


class UStaticMeshComponent;
class UTimelineComponent;
class UCurveFloat;
class UMaterialInstanceDynamic;
class USphereComponent;

UCLASS()
class COOPGAME_API AMyIceEffectActor : public AActor
{
	GENERATED_BODY()
	
	
public:	
	// Sets default values for this actor's properties
	AMyIceEffectActor();

	
	
	UFUNCTION()
	void SetMesh(UStaticMeshComponent * MyMesh);
	
	
	UTimelineComponent * MyTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat * fCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float SnowBias;
	

	/** Declare our delegate function to be binded with TimelineFloatReturn(float value) */
	FOnTimelineFloat InterpFunction{};

	/** Declare our delegate function to be binded with OnTimelineFinished() */
	FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
		void TimelineFloatReturn(float value);

	UFUNCTION()
		void OnTimelineFinished();

	UPROPERTY()
		TArray<UMaterialInstanceDynamic *>  mats;

	/** called when mesh hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void TriggerEffect();
	
	virtual void BeginPlay() override;
	
protected:

	UPROPERTY()
	TArray<UMaterialInstanceDynamic *>  MaterialInstanceComps;
	
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UStaticMeshComponent * MeshEffectApplied;
	// Called when the game starts or when spawned

};
