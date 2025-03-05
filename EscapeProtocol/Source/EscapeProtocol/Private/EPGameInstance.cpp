// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameInstance.h"

UEPGameInstance::UEPGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> SpawnItemDataObject(TEXT("/Game/Item/ItemDropTable.ItemDropTable"));

	if (SpawnItemDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnItemDataObject Find Succeed!"));
		SpawnItemDataTable = SpawnItemDataObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterItemDataObject(TEXT("/Game/Item/CharacterDataField.CharacterDataField"));
	if (CharacterItemDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterItemDataObject Find Succeed!"));
		CharacterItemDataTable = CharacterItemDataObject.Object;
	}

	
	if (CharacterItemDataTable)
	{
		static const FString ContextString(TEXT("Character1"));
		CharacterItemData = CharacterItemDataTable->FindRow<FEPCharacterDataRow>(FName("CharacterData1"), ContextString);

		CurrnetHandGunAmmo = CharacterItemData->HandGunAmmo;
		CurrnetRifleAmmo = CharacterItemData->RifleAmmo;
		CurrnetSnipeAmmo = CharacterItemData->SnipeAmmo;
		CurrnetShotGunAmmo = CharacterItemData->ShotGunAmmo;
		HasPotion = CharacterItemData->PotionAmount;
	}
	

}

