// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPEnemyCharacter.h"
#include "EPRiktor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEPROTOCOL_API AEPRiktor : public AEPEnemyCharacter
{
	GENERATED_BODY()
	
public:
    AEPRiktor();

protected:
    // **콤보 공격 관련 변수들**

    /** 현재 콤보 단계 (0부터 시작) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo")
    int32 CurrentComboIndex;

    /** 최대 콤보 횟수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 MaxComboCount;

    /** 다음 콤보로 넘어갈 수 있는지 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo")
    bool bCanCombo;

    /** 콤보 공격에 사용할 몽타주들 (0 ~ N) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    TArray<UAnimMontage*> ComboMontages;

    // 공격 진행 중인지 판단 (부모에 있는 bIsAttacking을 활용할 수도 있음)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo")
    bool bIsComboAttacking;

    // 공격 시도 (AI에서 호출할 함수)
    virtual void Attack() override;

    // 실제 콤보 공격 실행
    void PerformComboAttack();

    // 몽타주가 끝났을 때 처리
    UFUNCTION()
    void OnComboMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    // 다음 콤보 입력(혹은 AI 로직)에 따라 콤보를 이어갈지 여부 결정
    void TryNextCombo();

public:
    // 콤보를 강제로 끊거나 초기화하고 싶을 때 호출
    UFUNCTION(BlueprintCallable, Category = "Combo")
    void ResetCombo();
};
