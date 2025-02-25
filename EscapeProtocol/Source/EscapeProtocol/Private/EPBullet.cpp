// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBullet.h"

// Sets default values
AEPBullet::AEPBullet()
{

	Senen = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Senen);

	Range = 1000.0f;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEPBullet::BeginPlay()
{
	Super::BeginPlay();

	FHitResult Hit;
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = (GetActorRotation().Vector() * Range) + StartTrace;
	DrawDebugLine(GetWorld(), StartTrace, EndTrace,FColor::Red,true);
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1))
	{

		if (Hit.GetActor()->ActorHasTag(FName("Enemy")))
		{
			UE_LOG(LogTemp, Warning, TEXT("&s"), Hit.GetActor()->GetFName());
			
		}
		
	}

}

// Called every frame
void AEPBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

