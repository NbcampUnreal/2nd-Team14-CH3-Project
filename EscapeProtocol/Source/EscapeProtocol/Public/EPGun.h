// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPBaseWeapon.h"
#include "EPGun.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEPROTOCOL_API AEPGun : public AEPBaseWeapon
{
	GENERATED_BODY()

public:
	AEPGun();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet")
	int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet")
	int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet");
	float FireDelay;
	FTimerHandle FireDelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet");
	bool FireReady;

public:
	UFUNCTION(BlueprintPure)
	int32 GetAmmo();
	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo();

	UFUNCTION(BlueprintCallable)
	void SetAmmo(int32 Value);
	UFUNCTION(BlueprintCallable)
	void SetMaxAmmo(int32 Value);

	virtual void FireGun() override;
	
	virtual void ReloadGun() override;

	virtual EWeaponType GetWeaponType() const override;

	void SetFireReady();
};
