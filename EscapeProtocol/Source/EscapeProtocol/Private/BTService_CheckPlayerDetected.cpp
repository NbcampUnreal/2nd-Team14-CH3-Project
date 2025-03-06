#include "BTService_CheckPlayerDetected.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EPAIController.h"
#include "EPEnemyCharacter.h"
#include "EPCharacter.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_CheckPlayerDetected::UBTService_CheckPlayerDetected()
{
    bNotifyTick = true; // TickNode가 호출되도록 설정
    bNotifyBecomeRelevant = false;
    bNotifyCeaseRelevant = false;

    NodeName = TEXT("Check Player Detected");
}

void UBTService_CheckPlayerDetected::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // 1) Blackboard 컴포넌트 가져오기
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return;

    // 2) AIController 가져오기
    AEPAIController* AIController = Cast<AEPAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return;

    // 3) AI Pawn(EPEnemyCharacter) 가져오기
    AEPEnemyCharacter* AICharacter = Cast<AEPEnemyCharacter>(AIController->GetPawn());
    if (!AICharacter) return;

    // 4) 블랙보드에서 PatrolSpeed, ChaseSpeed 가져오기
    float CurrentPatrolSpeed = BlackboardComp->GetValueAsFloat(PatrolSpeedKey.SelectedKeyName);
    float CurrentChaseSpeed = BlackboardComp->GetValueAsFloat(ChaseSpeedKey.SelectedKeyName);

    // 5) TargetActor 키에서 AEPCharacter 가져오기
    UObject* TargetObj = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
    AEPCharacter* TargetChar = Cast<AEPCharacter>(TargetObj);

    // 초기값: 감지 실패(= PatrolSpeed)
    bool bDetected = false;
    float NewSpeed = CurrentPatrolSpeed;

    if (TargetChar)
    {
        float Dist = FVector::Dist(AICharacter->GetActorLocation(), TargetChar->GetActorLocation());

        // (1) 타겟이 살아 있고, (2) 거리 <= CheckRange, (3) FOVAngle 안에 있으면 감지 성공
        if (!TargetChar->IsDead() && Dist <= CheckRange && IsInFOV(AICharacter, TargetChar) && HasLineOfSight(AICharacter, TargetChar))
        {
            bDetected = true;
            NewSpeed = CurrentChaseSpeed;
        }
    }

    // 감지 여부를 Blackboard에 기록
    BlackboardComp->SetValueAsBool(PlayerDetectedKey.SelectedKeyName, bDetected);

    // 감지 실패면 TargetActor 지우기
    if (!bDetected)
    {
        BlackboardComp->ClearValue(TargetActorKey.SelectedKeyName);
    }

    // 이동 속도 적용
    AICharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

bool UBTService_CheckPlayerDetected::IsInFOV(AEPEnemyCharacter* AICharacter, AEPCharacter* TargetChar) const
{
    if (!AICharacter || !TargetChar) return false;

    // AI가 바라보는 전방 벡터
    FVector Forward = AICharacter->GetActorForwardVector().GetSafeNormal();

    // AI → 타겟 방향 벡터
    FVector ToTarget = (TargetChar->GetActorLocation() - AICharacter->GetActorLocation()).GetSafeNormal();

    // 내적(Dot Product)
    float Dot = FVector::DotProduct(Forward, ToTarget);

    // FOVAngle의 절반각도 (예: FOV=90 → 절반=45)
    float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(FOVAngle * 0.5f));

    // Dot >= Cos(절반 FOV)이면 시야각 안에 있음
    return (Dot >= CosHalfFOV);
}

bool UBTService_CheckPlayerDetected::HasLineOfSight(AEPEnemyCharacter* AICharacter, AEPCharacter* TargetChar) const
{
    if (!AICharacter || !TargetChar) return false;

    FVector Start = AICharacter->GetActorLocation() + FVector(0.f, 0.f, 50.f);
    FVector End = TargetChar->GetActorLocation() + FVector(0.f, 0.f, 50.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(AICharacter);
    Params.AddIgnoredActor(TargetChar);

    bool bHit = AICharacter->GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Visibility, Params
    );

    if (!bHit)
    {
        // 아무것도 안 맞았다 → 장애물 없음
        return true;
    }
    else
    {
        // 맞은 액터가 TargetChar라면 OK, 아니면 장애물
        return (Hit.GetActor() == TargetChar);
    }
}
