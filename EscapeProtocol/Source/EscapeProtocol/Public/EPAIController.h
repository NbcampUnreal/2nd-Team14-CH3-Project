#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EPAIController.generated.h"


UCLASS()
class ESCAPEPROTOCOL_API AEPAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEPAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void SetPatrolRadius(float Radius);
	void SetCombatState(bool State);
	void SetTarget(AActor* Target);
	void SetChaseSpeed(float CSpeed);
	void SetPatrolSpeed(float PSpeed);

private:
	UPROPERTY(VisibleAnywhere, Category="AI")
	UBehaviorTreeComponent* BehaviorTreeComp;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	// Blackboard와 Behavior Tree 에셋을 에디터에서 설정할 수 있도록 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardData* BlackboardAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTreeAsset;
};
