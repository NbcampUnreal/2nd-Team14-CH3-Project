// Fill out your copyright notice in the Description page of Project Settings.


#include "EPItemPotion.h"
#include "EPInventoryComponent.h"
#include "EPCharacter.h"
AEPItemPotion::AEPItemPotion()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEPItemPotion::BeginPlay()
{
	Super::BeginPlay();
}

void AEPItemPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEPItemPotion::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		//add inventory Potion
		AEPCharacter* EPCharacter = Cast<AEPCharacter>(Activator);
		if (EPCharacter)
		{
			EPCharacter->InventoryComponent->Potion++;
		}
		DestroyItem();
	}
}


