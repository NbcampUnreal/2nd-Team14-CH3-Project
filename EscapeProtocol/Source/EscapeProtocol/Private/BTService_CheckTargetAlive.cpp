#include "BTService_CheckTargetAlive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EPCharacter.h"

UBTService_CheckTargetAlive::UBTService_CheckTargetAlive()
{
    bNotifyTick = true;
    Interval = 0.1f;
    RandomDeviation = 0.1f;
    NodeName = TEXT("Check Target Alive");
}

void UBTService_CheckTargetAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        // Blackboard에 등록된 TargetActor를 가져옴
        UObject* TargetObject = BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName);
        AEPCharacter* TargetCharacter = Cast<AEPCharacter>(TargetObject);
        if (TargetCharacter && TargetCharacter->IsDead())
        {
            // 대상이 죽었으면 Blackboard에서 제거
            BlackboardComp->ClearValue(BlackboardKey.SelectedKeyName);
        }
    }


}


