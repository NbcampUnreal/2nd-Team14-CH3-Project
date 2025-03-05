#include "BTService_EPCheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EPAIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UBTService_EPCheckDistance::UBTService_EPCheckDistance()
{
    NodeName = TEXT("Check Distance to Player");
    bNotifyBecomeRelevant = true;
}

void UBTService_EPCheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* Player = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
        AActor* SelfActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());

        if (Player && SelfActor)
        {
            float Distance = FVector::Dist(Player->GetActorLocation(), SelfActor->GetActorLocation());
            float AttackRange = BlackboardComp->GetValueAsFloat(TEXT("AttackRange"));

            // 공격 범위(예: 200.0f) 내인지 확인
            bool bInRange = (Distance <= AttackRange);

            BlackboardComp->SetValueAsBool("IsInAttackRange", bInRange);
        }
    }
}


