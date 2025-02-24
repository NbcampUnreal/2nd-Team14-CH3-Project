// Fill out your copyright notice in the Description page of Project Settings.


#include "EPAssaultRifle.h"

AEPAssaultRifle::AEPAssaultRifle()
{
	Ammo = 30;
	MaxAmmo = 30;
	FireReady = true;
	WeaponType = EWeaponType::Rifle;
}

void AEPAssaultRifle::FireGun()
{
	
	if (FireReady)
	{
		Super::FireGun();
		FireReady = Super::FireReady;
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
