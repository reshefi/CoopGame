// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyIceEffectActor.h"
#include "MyStaticMeshWithEffectsTest.generated.h"

class AMyIceEffectActor;
class UPrimitiveComponent;


UCLASS()
class COOPGAME_API AMyStaticMeshWithEffectsTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyStaticMeshWithEffectsTest();

	UPROPERTY(EditAnywhere, Category = "Timeline")
	AMyIceEffectActor *  IceEffect;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UStaticMeshComponent * MeshObject;

	/** called when mesh hits something */
//	UFUNCTION()
//		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	

	
	
};
