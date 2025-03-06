#pragma once

#include "CoreMinimal.h"
#include "UObject/ScriptDelegates.h"
#include "GameFramework/Character.h"
#include "EPEnemyInterface.h"
#include "EPEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);

UCLASS()
class ESCAPEPROTOCOL_API AEPEnemyCharacter : public ACharacter, public IEPEnemyInterface
{
	GENERATED_BODY()

private:
	bool bIsAttacking;
	FTimerHandle CombatTimerHandle;
	FTimerHandle DeathTimerHandle;
	bool bIsInCombat;
	bool PlayerDetected;
	float CombatDuration;
	void StartCombat();
	void EndCombat();
	
	

public:
	AEPEnemyCharacter();

	void BeginPlay();

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackEnded OnAttackEnded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* DeathMontage;
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	// DeathMontage 종료 시 호출될 함수
	UFUNCTION()
	virtual float GetHealth() const override;
	UFUNCTION(BlueprintPure, Category = "State")
	virtual float GetAttackerPower() const override;
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool GetIsAttacking() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Information|State")
	bool IsDead() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack();
	virtual void DropLoot();

	// 사망 처리를 담당하는 함수 (DeathMontage가 끝난 후 호출)
	UFUNCTION(BlueprintCallable, Category = "Death")
	void HandleDeath();

	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	float PatrolSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	float ChaseSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	float AttackRange;


	float Health;	
	float MaxHealth;
	float AttackDamage;
	float patrolRadius;
	FName HeadBoneName;
	bool bIsDead;


	

};
