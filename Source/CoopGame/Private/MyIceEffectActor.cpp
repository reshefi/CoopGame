// Fill out your copyright notice in the Description page of Project Settings.

#include "MyIceEffectActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Array.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialLayersFunctions.h"
#include "NoExportTypes.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AMyIceEffectActor::AMyIceEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshEffectApplied = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringArmComp"));
	MeshEffectApplied->SetupAttachment(RootComponent);

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	SnowBias = 0.01;
	//MeshEffectApplied->OnComponentHit.AddDynamic(this, &AMyIceEffectActor::OnHit);	// set up a notification for when this component hits something blocking
	if (fCurve == nullptr) {
		fCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/IceEffect.IceEffect"));
	}
	//MyTimeline->ReregisterComponent();
	UE_LOG(LogTemp, Warning, TEXT("Value is: ff"));
}

// Sets default values
void AMyIceEffectActor::SetMesh(UStaticMeshComponent * NewMeshEffectApplied)
{
	//MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	//InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	//TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
	this->MeshEffectApplied = NewMeshEffectApplied;
	MyTimeline->PrimaryComponentTick.Target = MyTimeline;
	MyTimeline->RegisterComponent();
	//MeshEffectApplied->SetupAttachment(RootComponent);
	
	//MyTimeline->RegisterAllComponentTickFunctions(true);
	//Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Value is: f"));
	//MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	//MeshEffectApplied->OnComponentHit.AddDynamic(this, &AMyIceEffectActor::OnHit);
	
}

void AMyIceEffectActor::AddSkeletalMesh(TArray<USkeletalMeshComponent *> SMesh)
{
	UE_LOG(LogTemp, Warning, TEXT("Before Add"));
	if (SMesh.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("After Add"));
		SkeletalMeshApplied = SMesh;
		MyTimeline->PrimaryComponentTick.Target = MyTimeline;
		MyTimeline->RegisterComponent();
		UE_LOG(LogTemp, Warning, TEXT("Skeletal Meshes set"));
	}
}

/** This is the function which runs on the float variable according to the curve */
void AMyIceEffectActor::TimelineFloatReturn(float value)
{
	TArray<FMaterialParameterInfo>  MatInfo;
	TArray<FGuid>  GuidInfo;
	
	UE_LOG(LogTemp, Warning, TEXT("Value is: %f"), value);
	float SnowValue = -5;
	//UMaterialInstance * BaseMat;

	for (int32 MatIndex = 0; MatIndex < MaterialInstanceComps.Num(); ++MatIndex) 
	{
		MaterialInstanceComps[MatIndex]->GetAllScalarParameterInfo(MatInfo, GuidInfo);
		//BaseMat = Cast<UMaterialInstance>(MaterialInstanceComps[MatIndex]->GetBaseMaterial();
		//BaseMat->GetAllScalarParameterInfo(MatInfo, FGuidId);
		for (int32 ParamIndex = 0 ; ParamIndex < MatInfo.Num() ; ++ParamIndex) {
			UE_LOG(LogTemp, Warning, TEXT("Value is: %s"), *MatInfo[ParamIndex].Name.ToString());
			if (MatInfo[ParamIndex].Name.ToString() == "SnowBias") {
				MaterialInstanceComps[MatIndex]->GetScalarParameterValue(MatInfo[ParamIndex], SnowValue);
				if (SnowValue == 2.5) {
					return; //effect already applied
				}
			}
		}
		MaterialInstanceComps[MatIndex]->SetScalarParameterValue("SnowBias", value);
	}
}

void AMyIceEffectActor::OnTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Just Finished"));
	MaterialInstanceComps.Empty();
}

/*void AMyIceEffectActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
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
} */

void AMyIceEffectActor::OnLaserBeamStaticMeshHit()
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

void AMyIceEffectActor::OnLaserBeamSkeletalMeshHit()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit I AM Indeed mmmh ..!!"));
	UMaterialInstanceDynamic * Mat = nullptr;
	MaterialInstanceComps.Empty();
	int32 NumOfMeshes = this->SkeletalMeshApplied.Num();
	for (int32 SMeshIndex = 0; SMeshIndex < NumOfMeshes; ++SMeshIndex)
	{
		int32 mCnt = SkeletalMeshApplied[SMeshIndex]->GetNumMaterials();
		for (int i = 0; i < mCnt; i++) {
			Mat = SkeletalMeshApplied[SMeshIndex]->CreateAndSetMaterialInstanceDynamic(i);
			MaterialInstanceComps.Add(Mat);
		}
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


