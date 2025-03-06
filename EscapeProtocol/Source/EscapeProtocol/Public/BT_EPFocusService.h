#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BT_EPFocusService.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API UBT_EPFocusService : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBT_EPFocusService();

protected:
    // 서비스가 활성화될 때(브랜치에 들어올 때) 한 번 호출
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 서비스가 비활성화될 때(브랜치에서 벗어날 때) 한 번 호출
    virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 매 프레임(틱)마다 실행
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
