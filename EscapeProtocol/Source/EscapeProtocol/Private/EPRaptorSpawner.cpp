#include "EPRaptorSpawner.h"
#include "EPRaptor.h"
#include "EPAIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" // 플레이어 Pawn을 얻기 위한 헤더
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


AEPRaptorSpawner::AEPRaptorSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
    SpawnTime = 3.0f;
}

void AEPRaptorSpawner::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay 시에 1초 간격으로 SpawnRaptor 함수를 호출하는 타이머를 설정합니다.
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEPRaptorSpawner::SpawnRaptor, SpawnTime, true);
	
}

void AEPRaptorSpawner::SpawnRaptor()
{
    if (RaptorClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AEPRaptor* SpawnedRaptor = GetWorld()->SpawnActor<AEPRaptor>(RaptorClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedRaptor)
        {
            // 자동 Possess가 안 될 경우 수동으로 컨트롤러 생성
            SpawnedRaptor->SpawnDefaultController();

            AEPAIController* AIController = Cast<AEPAIController>(SpawnedRaptor->GetController());
            if (AIController)
            {
                // Blackboard 컴포넌트 가져오기 (컨트롤러에 BlackboardComp가 있다고 가정)
                UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
                if (AIController)
                {
                    // 플레이어 Pawn 가져오기 (플레이어 인덱스 0)
                    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
                    if (PlayerPawn)
                    {
                        // Blackboard의 TargetActor 키에 플레이어 Pawn 할당
                        BlackboardComp->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
                        // Blackboard의 PlayerDetected 키를 true로 설정
                        BlackboardComp->SetValueAsBool(TEXT("PlayerDetected"), true);

                        UE_LOG(LogTemp, Warning, TEXT("Updated Blackboard: TargetActor set to %s, PlayerDetected set to true"),
                            *PlayerPawn->GetName());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("PlayerPawn not found!"));
                    }

                    // Blackboard에 저장된 ChaseSpeed 값을 이동 속도로 할당
                    float ChaseSpeed = BlackboardComp->GetValueAsFloat(TEXT("ChaseSpeed"));
                    UE_LOG(LogTemp, Warning, TEXT("ChaseSpeed value from Blackboard: %f"), ChaseSpeed);

                    // AEPRaptor가 ACharacter를 상속받은 경우 CharacterMovement를 이용
                    ACharacter* RaptorCharacter = Cast<ACharacter>(SpawnedRaptor);
                    if (RaptorCharacter)
                    {
                        RaptorCharacter->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
                        UE_LOG(LogTemp, Warning, TEXT("Raptor movement speed set to %f"), ChaseSpeed);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Spawned Raptor is not a Character, cannot set movement speed."));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is null!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Spawned Raptor has no AIController assigned."));
            }
        }
    }
}


