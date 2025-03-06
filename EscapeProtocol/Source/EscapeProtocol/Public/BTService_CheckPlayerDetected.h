#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckPlayerDetected.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API UBTService_CheckPlayerDetected : public UBTService_BlackboardBase
{
    GENERATED_BODY()

public:
    UBTService_CheckPlayerDetected();

protected:
    /** 매 프레임(혹은 Interval마다) 호출되어 타겟의 상태를 체크한다 */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


public:
    /** 시야각(FOV). 예: 90 -> 좌우 45도 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float FOVAngle = 90.0f;
    /** 감지 범위(거리). 이 값을 넘어가면 감지 해제 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float CheckRange = 2000.0f;

    /** TargetActor와 함께, 살아있는지/거리 체크를 통해 PlayerDetected를 true/false로 세팅 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    FBlackboardKeySelector PlayerDetectedKey;

    /** 실제 추적/공격 대상. (예: 플레이어 캐릭터) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    FBlackboardKeySelector TargetActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    FBlackboardKeySelector ChaseSpeedKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    FBlackboardKeySelector PatrolSpeedKey;

private:
    /** AICharacter가 TargetChar를 시야각(FOVAngle) 안에서 바라보고 있는지 검사 */
    bool IsInFOV(class AEPEnemyCharacter* AICharacter, class AEPCharacter* TargetChar) const;
    /** 라인 트레이스로 벽이나 장애물 가로막 구현 **/
    bool HasLineOfSight(AEPEnemyCharacter* AICharacter, AEPCharacter* TargetChar) const;


};

