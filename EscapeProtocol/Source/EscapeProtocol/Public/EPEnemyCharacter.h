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

public:
	AEPEnemyCharacter();

	void BeginPlay();

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackEnded OnAttackEnded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION(BlueprintPure, Category = "State")
	virtual float GetHealth() const override;
	UFUNCTION(BlueprintPure, Category = "State")
	virtual float GetAttackerPower() const override;
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool GetIsAttacking() const;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack();
	virtual void DropLoot();
protected:
	float Health;
	float MaxHealth;
	float AttackDamage;
	float patrolRadius;


	

};
