#include "BTTask_EPAttack.h"
#include "EPAIController.h"
#include "Kismet/GameplayStatics.h"

UBTTask_EPAttack::UBTTask_EPAttack()
{
	NodeName = TEXT("Attack");
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;		// 노드 객체를 AI몹마다 만들도록 함.


}

EBTNodeResult::Type UBTTask_EPAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	EnemyCharacter = Cast<AEPEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (EnemyCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	EnemyCharacter->OnAttackEnded.AddUniqueDynamic(this, &UBTTask_EPAttack::OnAttackEnded);

	EnemyCharacter->Attack();
	CachedOwnerComp = &OwnerComp;

	return EBTNodeResult::InProgress;
}

void UBTTask_EPAttack::OnAttackEnded()
{
	if (EnemyCharacter)
	{
		EnemyCharacter->OnAttackEnded.RemoveDynamic(this, &UBTTask_EPAttack::OnAttackEnded);
	}

	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}


void UBTTask_EPAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (EnemyCharacter)
	{
		EnemyCharacter->OnAttackEnded.RemoveDynamic(this, &UBTTask_EPAttack::OnAttackEnded);
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}