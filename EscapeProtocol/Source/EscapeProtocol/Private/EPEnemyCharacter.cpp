#include "EPEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "EPCharacter.h"
#include "EPAIController.h"
#include "EPItemSpawnComponent.h"
#include "Components/CapsuleComponent.h"
#include "kismet/GameplayStatics.h"

AEPEnemyCharacter::AEPEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Enemy"));

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	MaxHealth = 200.0f;
	AttackDamage = 20.0f;
	Health = MaxHealth;
	patrolRadius = 1000.0f;
	CombatDuration = 5.0f;
	ChaseSpeed = 500.0f;
	PatrolSpeed = 150.0f;
	AttackRange = 100.0f;
	bIsInCombat = false;
	HeadBoneName = FName(TEXT("head"));
	bIsDead = false;
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
		AIController->SetAttackRange(AttackRange);
	}
}

bool AEPEnemyCharacter::GetIsAttacking() const
{
	return bIsAttacking;
}

bool AEPEnemyCharacter::IsDead() const
{
	return bIsDead;
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
	if (bIsDead)
	{
		return 0.f;
	}


	float ModifiedDamage = DamageAmount;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (PointDamageEvent && PointDamageEvent->HitInfo.BoneName == HeadBoneName)
		{
			ModifiedDamage *= 4.0f;
			UE_LOG(LogTemp, Warning, TEXT("Headshot! Damage doubled to: %f"), ModifiedDamage);
		}
	}


	ModifiedDamage = Super::TakeDamage(ModifiedDamage, DamageEvent, EventInstigator, DamageCauser);
	
	Health -= ModifiedDamage;

	UE_LOG(LogTemp, Warning, TEXT("Enemy took damage: %f, Remaining Health: %f"), ModifiedDamage, Health);

	// 체력이 0 이하라면 사망 처리
	if (Health <= 0.f)
	{
		Health = 0.0f;
		bIsDead = true;

		if (DeathMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				float MontageDuration = AnimInstance->Montage_Play(DeathMontage, 1.f);
				UE_LOG(LogTemp, Warning, TEXT("Playing DeathMontage, Duration: %f"), MontageDuration);

				// ACharacter가 아니라면, PawnMovementComponent를 찾아서 이동을 중지합니다.
				if (AEPAIController* AIController = Cast<AEPAIController>(GetController()))
				{
					AIController->UnPossess();
				}

				float DelayTime = 2.0f; // 이 값을 조정해서 노티파이가 확실히 발생하는지 확인
				GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEPEnemyCharacter::HandleDeath, DelayTime, false);
			}
		}
		else
		{
			// AnimInstance를 중지시키고 RagDoll 활성화
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetSimulatePhysics(true);
			// 애니메이션 모드를 물리 시뮬레이션에 맞게 변경
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

			AEPAIController* AIController = Cast<AEPAIController>(GetController());
			if (AIController)
			{
				AIController->UnPossess();
			}
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetActorTickEnabled(false);

			DropLoot();
		}
	}

	return ModifiedDamage;
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
				
				AEPCharacter* PlayerCharacter = Cast<AEPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				AEPAIController* AIController = Cast<AEPAIController>(GetController());
				if (PlayerCharacter)
				{
					if (AIController)
					{
						AIController->SetIsAttacking(bIsAttacking);
					}
					float DamageAmount = GetAttackerPower();
					UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount, GetController(), this, UDamageType::StaticClass());
					
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
	StartCombat();

	if (Montage == AttackMontage)
	{
		bIsAttacking = false;
		OnAttackEnded.Broadcast();
		AEPAIController* AIController = Cast<AEPAIController>(GetController());
		if (AIController)
		{
			AIController->SetIsAttacking(bIsAttacking);
		}
	}
}


void AEPEnemyCharacter::DropLoot()
{
	if (UEPItemSpawnComponent* ItemSpawner = FindComponentByClass<UEPItemSpawnComponent>())
	{
		ItemSpawner->SpawnItem(GetActorLocation());
	}
}

void AEPEnemyCharacter::HandleDeath()
{

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		// DeathMontage의 총 재생 길이를 가져옵니다.
		float MontageLength = DeathMontage->GetPlayLength();
		// 마지막 프레임에 가깝게 위치를 설정하고 재생 속도를 0으로 만듭니다.
		AnimInstance->Montage_SetPosition(DeathMontage, MontageLength - 0.01f);
		AnimInstance->Montage_SetPlayRate(DeathMontage, 0.0f);
	}

	// 충돌 및 이동 컴포넌트를 제거하여 더 이상 이동하지 않게 합니다.
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Capsule->DestroyComponent();
	}

	// 스켈레탈 메시의 애니메이션 업데이트를 중지합니다.
	GetMesh()->bPauseAnims = true;



	SetActorTickEnabled(false);
	DropLoot();
}
