// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGoal.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "EPGameState.h"

// Sets default values
AEPGoal::AEPGoal()
{
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(Scene);

    Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    // 겹침만 감지하는 프로파일 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AEPGoal::OnItemOverlap);

    bDoorOpen = false;

}

void AEPGoal::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // OtherActor가 플레이어인지 확인 ("Player" 태그 활용)
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        // 아이템 사용 (획득) 로직 호출
        bDoorOpen = true;

        AEPGameState* GameState = GetWorld()->GetGameState<AEPGameState>();
        if (GameState)
        {
            // TimerDelegate로 EndGame 함수 바인딩
            FTimerDelegate TimerDelegate;
            TimerDelegate.BindUFunction(GameState, FName("EndGame"));

            // 5초 후에 EndGame 호출
            GetWorld()->GetTimerManager().SetTimer(GameState->LevelTimerHandle, TimerDelegate, 9.2f, false);
        }
        

    }
}

