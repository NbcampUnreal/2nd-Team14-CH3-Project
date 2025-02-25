#include "EPAIController.h"
#include "Navigation/PathFollowingComponent.h"


AEPAIController::AEPAIController()
{
	// 블랙보드 및 비헤이비어 트리 컴포넌트 생성
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	// 블랙보드 컴포넌트 생성
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

    if (InPawn)
    {
        // 블랙보드 컴포넌트 설정
        if (UseBlackboard(BlackboardAsset, BlackboardComp))
        {
            // 블랙보드가 설정되었으면 Behavior Tree를 실행
            RunBehaviorTree(BehaviorTreeAsset);
        }
    }
}

void AEPAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEPAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
}


void AEPAIController::SetPatrolRadius(float Radius)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsFloat(TEXT("PatrolRadius"), Radius);
	}
}

void AEPAIController::SetCombatState(bool State)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(TEXT("PlayerDetected"), State);
    }
}

void AEPAIController::SetTarget(AActor* Target)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Target);

    }
}
