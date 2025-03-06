#include "BT_EPFocusService.h"
#include "EPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBT_EPFocusService::UBT_EPFocusService()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	NodeName = TEXT("Focus on Target");

}

void UBT_EPFocusService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBT_EPFocusService::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	// 서비스가 끝날 때 Focus 해제
	if (AEPAIController* AIController = Cast<AEPAIController>(OwnerComp.GetAIOwner()))
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

void UBT_EPFocusService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEPAIController* AIController = Cast<AEPAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	// Blackboard에서 Actor 가져오기
	FName KeyName = BlackboardKey.SelectedKeyName;
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(KeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor)
	{
		AIController->SetFocus(TargetActor);
	}
	else
	{
		// 타겟이 없으면 Focus 해제
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

}


