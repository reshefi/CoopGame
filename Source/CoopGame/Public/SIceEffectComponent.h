// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "SIceEffectComponent.generated.h"

class UCurveFloat;
class UTimelineComponent;
class UMaterialInstanceDynamic;
class UStaticMeshComponent;
class UPrimitiveComponent;
class UEngineTypes;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USIceEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USIceEffectComponent();

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent * MyTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat * fCurve;
	
	UPROPERTY(Category = "Components", BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent * MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float SnowBiasOffset;

	/** Declare our delegate function to be binded with TimelineFloatReturn(float value) */
	FOnTimelineFloat InterpFunction{};

	/** Declare our delegate function to be binded with OnTimelineFinished() */
	FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
	void TimelineFloatReturn(float value);
	
	UFUNCTION()
	void OnTimelineFinished();


	UPROPERTY()
	TArray <UMaterialInstanceDynamic *>   Mats;

	/** called when mesh hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

protected:
	

	virtual void BeginPlay() override;


public:	
	UFUNCTION()
	void PlayEffect(TArray<UMaterialInstanceDynamic *> target);

	
};
