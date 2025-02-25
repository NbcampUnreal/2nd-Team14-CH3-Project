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
}

void AEPEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsAttacking = false;

	AEPAIController* AIController = Cast<AEPAIController>(GetController());
	if (AIController)
	{
		AIController->SetPatrolRadius(patrolRadius);
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
				bIsAttacking = true;

				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AEPEnemyCharacter::OnAttackMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
			}
		}
	}
	AEPCharacter* PlayerCharacter = Cast<AEPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (PlayerCharacter)
	{
		float DamageAmount = GetAttackerPower();
		UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount, GetController(), this, UDamageType::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("Player takeDamage : %f"), DamageAmount);
	}

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