// Fill out your copyright notice in the Description page of Project Settings.


#include "EPAssaultRifle.h"
#include "Components/SkeletalMeshComponent.h"
#include "EPBullet.h"
#include "Kismet/GameplayStatics.h"

AEPAssaultRifle::AEPAssaultRifle()
{
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle"));

	if (SkeletalMeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}


	

	Ammo = 30;
	MaxAmmo = 30;
	FireReady = true;
	FireDelay = 1.0f;
	WeaponType = EWeaponType::Rifle;
	BulletBlueprint = nullptr;
}

void AEPAssaultRifle::FireGun()
{
	
	if (FireReady)
	{
		
		Super::FireGun();
		FireReady = Super::FireReady;
		MuzzleTransform = SkeletalMeshComp->GetSocketTransform(FName("Muzzle"), RTS_World);
		//ÅºÈ¯ »ý¼º
		//MuzzleEffect->rota
		GetWorld()->SpawnActor<AActor>(BulletBlueprint, MuzzleTransform);

		
		UE_LOG(LogTemp, Warning, TEXT("Rifle Fire"));


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle Fire Delay"));

	}
}

void AEPAssaultRifle::ReloadGun()
{
	Super::ReloadGun();
	UE_LOG(LogTemp, Warning, TEXT("Rifle Reload"));

}

EWeaponType AEPAssaultRifle::GetWeaponType() const
{
	return WeaponType;
}
