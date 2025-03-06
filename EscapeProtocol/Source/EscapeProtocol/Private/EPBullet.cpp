// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBullet.h"
#include "Particles/ParticleSystem.h"
#include "EPPlayerController.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AEPBullet::AEPBullet()
{

	Senen = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Senen);

	Range = 1000.0f;
	//MuzzleEffect = nullptr;
	HitEffect = nullptr;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleEffectObject(TEXT("ParticleSystem'/Game/Weapons/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Leather.P_Leather'"));

	if (MuzzleEffectObject.Succeeded())
	{
		MuzzleEffect = MuzzleEffectObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("MuzzleEffectObject.Succeeded"));
	}
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeTime = 3.0f;

}

// Called when the game starts or when spawned
void AEPBullet::BeginPlay()
{
	Super::BeginPlay();

	FHitResult Hit;
	FVector StartTrace = GetActorLocation();
	
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector Loc = CameraManager->GetCameraLocation();
	FVector Rot = CameraManager->GetActorForwardVector();

	FVector EndTrace = (Rot * Range) + Loc;
	DrawDebugLine(GetWorld(), StartTrace, EndTrace,FColor::Red,true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, StartTrace);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1))
	{

		if (Hit.GetActor()->ActorHasTag(FName("Enemy")))
		{

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

		}

	}
	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &AEPBullet::DestroyBullet, LifeTime, false);

}

// Called every frame
void AEPBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEPBullet::DestroyBullet()
{
	Destroy();
}

