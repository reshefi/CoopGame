// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USHealthComponent;
class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UMaterial;
class URadialForceComponent;

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	float DefaultHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent * BarrelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent * AreaOfDamage;

	UPROPERTY()
	URadialForceComponent * KineticRadialImpact;

	UFUNCTION()
	void OnDamageTaken (USHealthComponent * MyHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USHealthComponent* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	bool bExploded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	UParticleSystem * BlastEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	UMaterial * MaterialPostBlast;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	float  ForceUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	float  RadialForce;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
