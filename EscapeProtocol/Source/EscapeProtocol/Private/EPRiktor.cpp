#include "EPRiktor.h"
#include "EPCharacter.h"
#include "EPAIController.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "kismet/GameplayStatics.h"

AEPRiktor::AEPRiktor()
{
    // 기본값 설정
    MaxHealth = 10.0f;
    AttackDamage = 50.0f;
    Health = MaxHealth;
    patrolRadius = 2000.0f;
    CurrentComboIndex = 0;
    MaxComboCount = 3;
    PatrolSpeed = 200.0f;
    AttackRange = 300.0f;
    ChaseSpeed = 600.0f;
    bCanCombo = false;
    bIsComboAttacking = false;
}

void AEPRiktor::Attack()
{

    if (CurrentComboIndex == 0)
    {
        // 첫 공격
        PerformComboAttack();
    }
    else
    {
        TryNextCombo();
    }

}

void AEPRiktor::PerformComboAttack()
{

    
    // 방금 콤보 시작하므로
    bIsComboAttacking = true;
    bCanCombo = false;
    AEPCharacter* PlayerCharacter = Cast<AEPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (ComboMontages.IsValidIndex(CurrentComboIndex))
    {
        UAnimMontage* MontageToPlay = ComboMontages[CurrentComboIndex];
        if (MontageToPlay)
        {
            UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
            if (AnimInst)
            {
                float MontageDuration = AnimInst->Montage_Play(MontageToPlay, 1.0f);
                if (MontageDuration > 0.f)
                {
                    

                    // 몽타주가 끝날 때 실행할 델리게이트 바인딩
                    FOnMontageEnded EndDelegate;
                    EndDelegate.BindUObject(this, &AEPRiktor::OnComboMontageEnded);
                    AnimInst->Montage_SetEndDelegate(EndDelegate, MontageToPlay);

                    float DamageAmount = GetAttackerPower();
                    UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount * (CurrentComboIndex + 0.5), GetController(), this, UDamageType::StaticClass());
                }
                else
                {
                    // 재생 실패
                    ResetCombo();
                }
            }
        }
    }
    else
    {
        // 유효하지 않은 인덱스라면 콤보 초기화
        ResetCombo();
    }
}

void AEPRiktor::OnComboMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // 콤보 몽타주가 끝나면
    bIsComboAttacking = false;
    bCanCombo = true;

    // 자동 콤보: 현재 콤보 인덱스가 아직 MaxComboCount 미만이면 다음 공격
    CurrentComboIndex++;
    if (CurrentComboIndex >= MaxComboCount)
    {
        ResetCombo();
    }

    OnAttackEnded.Broadcast();
}

void AEPRiktor::TryNextCombo()
{
    // 이미 콤보 중이면, 다음 콤보로 넘어갈 수 있는지 체크
    if (bCanCombo)
    {

        // 다음 콤보 공격
        PerformComboAttack();
    }
    else
    {
        // 콤보 불가 상태면 그냥 무시
        UE_LOG(LogTemp, Warning, TEXT("Cannot combo now!"));
    }
}




void AEPRiktor::ResetCombo()
{
    CurrentComboIndex = 0;
    bCanCombo = false;
    bIsComboAttacking = false;
}
