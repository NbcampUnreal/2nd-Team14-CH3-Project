// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBlockDoor.h"
#include "EPGameState.h"

// Sets default values
AEPBlockDoor::AEPBlockDoor()
{

	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

}

// Called every frame
void AEPBlockDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEPGameState* GameState = GetWorld()->GetGameState<AEPGameState>();
	if (GameState && GameState->EPBook)
	{
		// 액터 삭제
		Destroy();
	}

}

