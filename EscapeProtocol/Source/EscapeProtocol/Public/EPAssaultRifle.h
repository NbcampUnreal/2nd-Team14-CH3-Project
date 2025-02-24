// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPGun.h"
#include "EPAssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEPROTOCOL_API AEPAssaultRifle : public AEPGun
{
	GENERATED_BODY()
	
public:

	AEPAssaultRifle();


	UFUNCTION(BlueprintCallable)
	virtual void FireGun() override;

	UFUNCTION(BlueprintCallable)
	virtual void ReloadGun() override;

	UFUNCTION(BlueprintCallable)
	virtual EWeaponType GetWeaponType() const override;
};
