// Fill out your copyright notice in the Description page of Project Settings.


#include "EPInventoryComponent.h"
#include "EPCharacter.h"
#include "EPGameInstance.h"

// Sets default values for this component's properties
UEPInventoryComponent::UEPInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	

	UGameInstance* Instance = GetWorld()->GetGameInstance();
	if (Instance)
	{
		UEPGameInstance* EPGameInstance = Cast<UEPGameInstance>(Instance);
		if (EPGameInstance)
		{

			HandGunAmmo = EPGameInstance->CurrnetHandGunAmmo;


			RifleAmmo = EPGameInstance->CurrnetRifleAmmo;


			SnipeAmmo = EPGameInstance->CurrnetSnipeAmmo;


			ShotGunAmmo = EPGameInstance->CurrnetShotGunAmmo;


			Potion = EPGameInstance->HasPotion;
		}
	}
	// ...
	
}


// Called every frame
void UEPInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEPInventoryComponent::UpdateData()
{
	UGameInstance* Instance = GetWorld()->GetGameInstance();
	if (Instance)
	{
		UEPGameInstance* EPGameInstance = Cast<UEPGameInstance>(Instance);
		if (EPGameInstance)
		{

			EPGameInstance->CurrnetHandGunAmmo = HandGunAmmo;


			EPGameInstance->CurrnetRifleAmmo = RifleAmmo;


			EPGameInstance->CurrnetSnipeAmmo = SnipeAmmo;


			EPGameInstance->CurrnetShotGunAmmo = ShotGunAmmo;


			EPGameInstance->HasPotion = Potion;
		}
	}
}

void UEPInventoryComponent::UsePotion()
{
	AActor* PlayerCharacter = GetOwner();

	if (PlayerCharacter)
	{
		AEPCharacter* EPPlayerCharacter = Cast<AEPCharacter>(PlayerCharacter);

		if (!EPPlayerCharacter || Potion <= 0)
		{
			return;
		}

		Potion--;
		EPPlayerCharacter->AddHealth(30.0f);
		UpdateData();

	}

	
}

