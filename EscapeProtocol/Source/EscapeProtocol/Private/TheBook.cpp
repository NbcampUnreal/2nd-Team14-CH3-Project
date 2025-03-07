// Fill out your copyright notice in the Description page of Project Settings.


#include "TheBook.h"
#include "Components/SphereComponent.h"
#include "EPGameState.h"
#include "EPBrokenDoor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATheBook::ATheBook()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // 겹침만 감지하는 프로파일 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // 루트 컴포넌트로 설정
    Collision->SetupAttachment(Scene);

    // 스태틱 메시 컴포넌트 생성 및 설정
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ATheBook::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ATheBook::OnItemEndOverlap);

}

void ATheBook::OnItemOverlap(
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
        ActivateItem(OtherActor);
    }
}

void ATheBook::OnItemEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
}

void ATheBook::ActivateItem(AActor* Activator)
{

    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            PickupSound,
            GetActorLocation()
        );
    }

    DestroyItem();

    AEPGameState* GameState = GetWorld()->GetGameState<AEPGameState>();
    if (GameState)
    {
        GameState->EPBook = true;  // EPBook을 true로 설정
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEPBrokenDoor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEPBrokenDoor* BrokenDoor = Cast<AEPBrokenDoor>(Actor);
        if (BrokenDoor)
        {
            // 액터의 SetActorVisibility 호출
            BrokenDoor->SetActorVisibility(true);  // 원하는 가시성 상태로 설정
        }
    }
}

void ATheBook::DestroyItem()
{
    Destroy();
}