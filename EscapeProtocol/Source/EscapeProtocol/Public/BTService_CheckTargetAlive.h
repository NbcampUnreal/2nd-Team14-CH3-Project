#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckTargetAlive.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API UBTService_CheckTargetAlive : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckTargetAlive();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
