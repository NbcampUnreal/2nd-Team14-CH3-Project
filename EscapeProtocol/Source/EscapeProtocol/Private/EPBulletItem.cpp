// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBulletItem.h"
#include "EPCharacter.h"
AEPBulletItem::AEPBulletItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEPBulletItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AEPBulletItem::BeginPlay()
{
	Super::BeginPlay();
}

void AEPBulletItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		//add inventory Bullet
		AEPCharacter* EPCharacter = Cast<AEPCharacter>(Activator);
		if (EPCharacter)
		{
			UEPInventoryComponent* Inventory = EPCharacter->InventoryComponent;
			if (Inventory)
			{
				switch (this->Type)
				{
				case EItemType::HandGun:
					Inventory->HandGunAmmo += Amount;
					break;
				case EItemType::Rifle:
					Inventory->RifleAmmo += Amount;
					break;
				case EItemType::Shotgun:
					Inventory->ShotGunAmmo += Amount;
					break;
				case EItemType::Snipe:
					Inventory->SnipeAmmo += Amount;
					break;
				default:
					break;
				}

				Inventory->UpdateData();
			}
			
		}
		
		DestroyItem();
	}
}

int32 AEPBulletItem::GetBulletAmount()
{
	return Amount;
}



