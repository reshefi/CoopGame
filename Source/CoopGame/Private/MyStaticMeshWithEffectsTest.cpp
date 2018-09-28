// Fill out your copyright notice in the Description page of Project Settings.

#include "MyStaticMeshWithEffectsTest.h"
#include "Components/StaticMeshComponent.h"
#include "MyIceEffectActor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"


// Sets default values
AMyStaticMeshWithEffectsTest::AMyStaticMeshWithEffectsTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	MeshObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshObject->SetupAttachment(RootComponent);
	IceEffect = CreateDefaultSubobject<AMyIceEffectActor>(TEXT("IceEffect"));
}


// Called when the game starts or when spawned
void AMyStaticMeshWithEffectsTest::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Began playing ..!!"));
	IceEffect->SetMesh(MeshObject);
	//IceEffect->BeginPlay();
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Changed Mesh ..!!"));
}



