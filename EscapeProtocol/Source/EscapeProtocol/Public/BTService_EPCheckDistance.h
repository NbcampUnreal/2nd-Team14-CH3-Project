#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_EPCheckDistance.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API UBTService_EPCheckDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_EPCheckDistance();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
