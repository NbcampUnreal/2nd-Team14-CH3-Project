// Fill out your copyright notice in the Description page of Project Settings.


#include "EPAssaultRifle.h"
#include "Components/SkeletalMeshComponent.h"

AEPAssaultRifle::AEPAssaultRifle()
{

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/WeaponResource/AR-15_style_rifle/SKM_ar_15_style_rifle.SKM_ar_15_style_rifle"));
	if (SkeletalMeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	Ammo = 30;
	MaxAmmo = 30;
	FireReady = true;
	FireDelay = 1.0f;
	WeaponType = EWeaponType::Rifle;
	//MuzzleLocation = SkeletalMeshComp->GetSocketLocation(TEXT("MuzzleSocket"));
}

void AEPAssaultRifle::FireGun()
{
	
	if (FireReady)
	{
		
		Super::FireGun();
		FireReady = Super::FireReady;
		MuzzleTransform = SkeletalMeshComp->GetSocketTransform(TEXT("MuzzleSocket"), RTS_World);
		//ÅºÈ¯ »ý¼º

		
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
