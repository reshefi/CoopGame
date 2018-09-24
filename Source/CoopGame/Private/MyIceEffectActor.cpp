// Fill out your copyright notice in the Description page of Project Settings.

#include "MyIceEffectActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Array.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AMyIceEffectActor::AMyIceEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	MeshEffectApplied = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringArmComp"));
	MeshEffectApplied->SetupAttachment(RootComponent);

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	SnowBias = 0.01;
	MeshEffectApplied->OnComponentHit.AddDynamic(this, &AMyIceEffectActor::OnHit);	// set up a notification for when this component hits something blocking
	if (fCurve == nullptr) {
		fCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/IceEffect.IceEffect"));
	}
}

// Sets default values
void AMyIceEffectActor::SetMesh(UStaticMeshComponent * NewMeshEffectApplied)
{
	this->MeshEffectApplied = NewMeshEffectApplied;

	//MeshEffectApplied->SetupAttachment(RootComponent);
	//MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	MeshEffectApplied->OnComponentHit.AddDynamic(this, &AMyIceEffectActor::OnHit);
	
}

/** This is the function which runs on the float variable according to the curve */
void AMyIceEffectActor::TimelineFloatReturn(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("Value is: %f"), value);
	for (int32 MatIndex = 0; MatIndex < MaterialInstanceComps.Num(); ++MatIndex) {
		MaterialInstanceComps[MatIndex]->SetScalarParameterValue("SnowBias", value);
	}
}

void AMyIceEffectActor::OnTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Just Finished"));
}

void AMyIceEffectActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{
	UE_LOG(LogTemp, Warning, TEXT("Hit I AM Indeed mmmh ..!!"));
	UMaterialInstanceDynamic * Mat = nullptr;
	MaterialInstanceComps.Empty();
	int32 mCnt = MeshEffectApplied->GetNumMaterials();
	for (int i = 0; i < mCnt; i++) {
		Mat = MeshEffectApplied->CreateAndSetMaterialInstanceDynamic(i);
		MaterialInstanceComps.Add(Mat);
	}
	TriggerEffect();
}


void AMyIceEffectActor::TriggerEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling for effect"));
	if (fCurve) {
		UE_LOG(LogTemp, Warning, TEXT("Calling for effect2"));
		// Add the float curve to the timeline and connect it to the interfunction's delegate.
		MyTimeline->AddInterpFloat(fCurve, InterpFunction, FName("SnowBias"));

		// Add our own timeline finished function
		MyTimeline->SetTimelineFinishedFunc(TimelineFinished);

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);
		MyTimeline->Play();
	}
}

// Called when the game starts or when spawned
void AMyIceEffectActor::BeginPlay()
{
	Super::BeginPlay();
}


