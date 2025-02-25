#include "EPAIController.h"
#include "Navigation/PathFollowingComponent.h"


AEPAIController::AEPAIController()
{
	// ������ �� �����̺�� Ʈ�� ������Ʈ ����
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	// ������ ������Ʈ ����
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

    if (InPawn)
    {
        // ������ ������Ʈ ����
        if (UseBlackboard(BlackboardAsset, BlackboardComp))
        {
            // �����尡 �����Ǿ����� Behavior Tree�� ����
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
