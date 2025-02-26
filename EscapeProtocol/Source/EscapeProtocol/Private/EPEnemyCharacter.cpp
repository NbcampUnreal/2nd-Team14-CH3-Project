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

	// ü���� 0 ���϶�� ��� ó��
	if (Health <= 0.f)
	{
		Health = 0.f;
		DropLoot(); // ����ǰ ���
		Destroy();  // �� ����
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

		// Blackboard �� ����
		AEPAIController* AIController = Cast<AEPAIController>(GetController());
		if (AIController)
		{
			AIController->SetCombatState(bIsInCombat);  // ������ ���� ����
		}
	}
	else
	{
		// ���� �� �߰� ���� �� CombatTime ����
		GetWorldTimerManager().ClearTimer(CombatTimerHandle);
		GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &AEPEnemyCharacter::EndCombat, CombatDuration, false);
	}
}

void AEPEnemyCharacter::EndCombat()
{

	// ���� ����� ���� ���̸� ���� ���Ḧ �����ϰ� Ÿ�̸� �缳��
	if (bIsAttacking)
	{
		GetWorldTimerManager().ClearTimer(CombatTimerHandle);
		GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &AEPEnemyCharacter::EndCombat, CombatDuration, false);
		return;
	}

	bIsInCombat = false;	// ���� �ð��� ������ ���� ����

	AEPAIController* AIController = Cast<AEPAIController>(GetController());
	if (AIController)
	{
		AIController->SetCombatState(bIsInCombat);  // ���� ����
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