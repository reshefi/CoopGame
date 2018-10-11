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

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent * BarrelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent * AreaOfDamage;

	UPROPERTY()
	URadialForceComponent * KineticRadialImpact;

	UFUNCTION()
	void OnDamageTaken (USHealthComponent * OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void Explode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USHealthComponent* HealthComp;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	bool bExploded;

	UPROPERTY(  Replicated, EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	UParticleSystem * BlastEffect;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	UMaterial * MaterialPostBlast;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	float  ForceUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Impact")
	float  RadialForce;

	UFUNCTION(NetMulticast , Reliable, WithValidation)
	void MulticastExplode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
