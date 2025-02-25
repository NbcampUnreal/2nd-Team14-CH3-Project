// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGun.h"

AEPGun::AEPGun()
{
	Ammo = 0;
	MaxAmmo = 0;
	FireDelay = 0;
	FireReady = true;

}

void AEPGun::FireGun()
{
	
	if (FireReady)
	{
		GetWorldTimerManager().SetTimer(
			FireDelayTimerHandle,
			this,
			&AEPGun::SetFireReady,
			FireDelay,
			false
		);
		FireReady = false;
		Super::FireGun();
		if (Ammo <= 0)
		{
			ReloadGun();
			return;
		}
		//ÆÄÆ¼Å¬
		Ammo--;

		UE_LOG(LogTemp, Warning, TEXT("Fire! %d / %d"), Ammo, MaxAmmo);
	}
	else
	{
		return;
	}
	
}

void AEPGun::ReloadGun()
{
	Super::ReloadGun();
	Ammo = MaxAmmo;

}

EWeaponType AEPGun::GetWeaponType() const
{
	return EWeaponType();
}

void AEPGun::SetFireReady()
{
	FireReady = true;
}

int32 AEPGun::GetAmmo()
{
	return Ammo;
}

int32 AEPGun::GetMaxAmmo()
{
	return MaxAmmo;
}

void AEPGun::SetAmmo(int32 Value)
{
	Ammo = Value;
}

void AEPGun::SetMaxAmmo(int32 Value)
{
	MaxAmmo = Value;
}


