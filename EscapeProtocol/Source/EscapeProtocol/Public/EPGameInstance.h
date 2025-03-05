// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EPCharacterDataRow.h"
#include "EPItemSpanwRow.h"
#include "EPWeaponDataRow.h"
#include "EPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEPROTOCOL_API UEPGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:



public:
	UEPGameInstance();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	UDataTable* SpawnItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	UDataTable* CharacterItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	UDataTable* WeaponDataTable;

	FEPCharacterDataRow* CharacterItemData;

	FEPItemSpawnRow* SpawnItemData;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 CurrnetHandGunAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 CurrnetRifleAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 CurrnetSnipeAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 CurrnetShotGunAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EPGameData")
	int32 HasPotion;
	
	
};
