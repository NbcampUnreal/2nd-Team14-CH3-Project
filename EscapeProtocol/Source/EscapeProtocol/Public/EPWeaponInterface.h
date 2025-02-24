// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EPWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEPWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	HandGun,
	Rifle,
	Sniper
};

/**
 * 
 */
class ESCAPEPROTOCOL_API IEPWeaponInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void FireGun() = 0;

	UFUNCTION()
	virtual void ReloadGun() = 0;

	UFUNCTION()
	virtual EWeaponType GetWeaponType()const = 0;


};
