// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBrokenDoor.h"

// Sets default values
AEPBrokenDoor::AEPBrokenDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	bIsVisible = false;

}

// Called when the game starts or when spawned
void AEPBrokenDoor::BeginPlay()
{
	Super::BeginPlay();
	SetActorVisibility(bIsVisible);
}

// Called every frame
void AEPBrokenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEPBrokenDoor::SetActorVisibility(bool bVisible)
{
	if (StaticMeshComp)
	{
		// MeshComponent의 가시성 설정
		StaticMeshComp->SetVisibility(bVisible);
		bIsVisible = bVisible; // 가시성 상태 업데이트
	}
}

