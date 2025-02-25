#pragma once

#include "CoreMinimal.h"
#include "EPEnemyCharacter.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EPAttack.generated.h"

class AEPEnemyCharacter;

UCLASS()
class ESCAPEPROTOCOL_API UBTTask_EPAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EPAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
	UBehaviorTreeComponent* CachedOwnerComp;

	UFUNCTION()
	void OnAttackEnded();

private:
	AEPEnemyCharacter* EnemyCharacter;

	
};
