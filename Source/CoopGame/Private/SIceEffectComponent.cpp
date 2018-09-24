// Fill out your copyright notice in the Description page of Project Settings.

#include "SIceEffectComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Array.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/PrimitiveComponent.h"


// Sets default values for this component's properties
USIceEffectComponent::USIceEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringArmComp"));
	
	//MeshComponent->SetupAttachment(RootComponent);
	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	if (MyTimeline) {
		InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
		fCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/IceEffect.IceEffect"));
	}
	MeshComponent->OnComponentHit.AddDynamic(this, &USIceEffectComponent::OnHit);

	//mats.AddUninitialized();
	SnowBiasOffset = 0.01;

}



void USIceEffectComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("Hit I AM mmmh ..!!"));
}


void USIceEffectComponent::TimelineFloatReturn(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("InTimeLineReturn 11"));
	for (int i = 0  ; i < Mats.Num() ; ++i ) {
		Mats[i]->SetScalarParameterValue("SnowBias", value);
	}
}

void USIceEffectComponent::OnTimelineFinished()
{

}

void USIceEffectComponent::PlayEffect(TArray<UMaterialInstanceDynamic *> target)
{
	
	Mats = TArray<UMaterialInstanceDynamic *>();
	Mats = target;
	if (fCurve) {
		
		// Add the float curve to the timeline and connect it to the interfunction's delegate.
		MyTimeline->AddInterpFloat(fCurve, InterpFunction, FName("SnowBiasOffset"));

		// Add our own timeline finished function
		MyTimeline->SetTimelineFinishedFunc(TimelineFinished);

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);
		MyTimeline->Play();
	}
}

void USIceEffectComponent::BeginPlay()
{
	MyTimeline->BeginPlay();
	Super::BeginPlay();
}
	
	//MyTimeline->bAutoActivate = false;
	//MyTimeline->bAutoRegister = true;




