#include "EPEnemyCharacter.h"
#include "EPCharacter.h"
#include "EPAIController.h"
#include "kismet/GameplayStatics.h"

AEPEnemyCharacter::AEPEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxHealth = 200.0f;
	AttackDamage = 20.0f;
	Health = MaxHealth;
	patrolRadius = 1000.0f;
	CombatDuration = 5.0f;
	ChaseSpeed = 500.0f;
	PatrolSpeed = 150.0f;
	bIsInCombat = false;
}

void AEPEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsAttacking = false;

	AEPAIController* AIController = Cast<AEPAIController>(GetController());
	if (AIController)
	{
		AIController->SetPatrolRadius(patrolRadius);
		AIController->SetChaseSpeed(ChaseSpeed);
		AIController->SetPatrolSpeed(PatrolSpeed);
	}
}

bool AEPEnemyCharacter::GetIsAttacking() const
{
	return bIsAttacking;
}

float AEPEnemyCharacter::GetHealth() const
{
	return Health;
}

float AEPEnemyCharacter::GetAttackerPower() const
{
	return AttackDamage;
}

float AEPEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Health -= DamageAmount;

	UE_LOG(LogTemp, Warning, TEXT("Enemy took damage: %f, Remaining Health: %f"), DamageAmount, Health);

	// 체력이 0 이하라면 사망 처리
	if (Health <= 0.f)
	{
		Health = 0.f;
		DropLoot(); // 전리품 드롭
		Destroy();  // 적 제거
	}

	return DamageAmount;
}


void AEPEnemyCharacter::Attack()
{
	
	if (bIsAttacking)
	{
		return;
	}

	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(AttackMontage, 1.0f);
			if (MontageDuration > 0.f)
			{
				StartCombat();
				bIsAttacking = true;
				AEPCharacter* PlayerCharacter = Cast<AEPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (PlayerCharacter)
				{
					float DamageAmount = GetAttackerPower();
					UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount, GetController(), this, UDamageType::StaticClass());
					UE_LOG(LogTemp, Warning, TEXT("Player takeDamage : %f"), DamageAmount);
				}


				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AEPEnemyCharacter::OnAttackMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
			}
		}
	}


}

void AEPEnemyCharacter::StartCombat()
{
	if (!bIsInCombat)
	{
		bIsInCombat = true;

		GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &AEPEnemyCharacter::EndCombat, CombatDuration, false);

		// Blackboard 값 변경
		AEPAIController* AIController = Cast<AEPAIController>(GetController());
		if (AIController)
		{
			AIController->SetCombatState(bIsInCombat);  // 강제로 감지 유지
		}
	}
	else
	{
		// 전투 중 추가 공격 시 CombatTime 리셋
		GetWorldTimerManager().ClearTimer(CombatTimerHandle);
		GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &AEPEnemyCharacter::EndCombat, CombatDuration, false);
	}
}

void AEPEnemyCharacter::EndCombat()
{

	// 공격 모션이 진행 중이면 전투 종료를 보류하고 타이머 재설정
	if (bIsAttacking)
	{
		GetWorldTimerManager().ClearTimer(CombatTimerHandle);
		GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &AEPEnemyCharacter::EndCombat, CombatDuration, false);
		return;
	}

	bIsInCombat = false;	// 일정 시간이 지나면 감지 종료

	AEPAIController* AIController = Cast<AEPAIController>(GetController());
	if (AIController)
	{
		AIController->SetCombatState(bIsInCombat);  // 감지 해제
	}

	UE_LOG(LogTemp, Warning, TEXT("Combat ended, returning to Patrol"));
}


void AEPEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		bIsAttacking = false;
		OnAttackEnded.Broadcast();
	}
}

void AEPEnemyCharacter::DropLoot()
{
}