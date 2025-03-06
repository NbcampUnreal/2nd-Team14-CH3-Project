// Fill out your copyright notice in the Description page of Project Settings.


#include "EPWeaponComponent.h"
#include "EPCharacter.h"
#include "EPGameInstance.h"
#include "EPInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UEPWeaponComponent::UEPWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	PrimaryComponentTick.bCanEverTick = true;
	FireReady = true;


	ConstructorHelpers::FClassFinder<AActor> RifleBulletClass(TEXT("/Game/Weapons/BP_ARBullet.BP_ARBullet_C"));
	if (RifleBulletClass.Succeeded())
	{
		RifleBullet = RifleBulletClass.Class;

	}


	// ...
}


// Called when the game starts
void UEPWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* PlayerActor = GetOwner();
	//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerActor)
	{
		Player = Cast<AEPCharacter>(PlayerActor);
		WeaponMesh = Player->WeaponMeshComponent;
		if (WeaponMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponMesh Succeed"));

		}
	}
	//AActor* OnwerActor = GetOwner();
	UGameInstance* Instance = GetWorld()->GetGameInstance();
	if (Instance)
	{
		UEPGameInstance* GameInstance = Cast<UEPGameInstance>(Instance);
		if (GameInstance->WeaponDataTable)
		{
			HandGunDataRow = GameInstance->WeaponDataTable->FindRow<FEPWeaponDataRow>(FName("HandGun"), FString("HandGunDataContext"));
			RifleDataRow = GameInstance->WeaponDataTable->FindRow<FEPWeaponDataRow>(FName("Rifle"), FString("RifleDataContext"));
			ShotGunDataRow = GameInstance->WeaponDataTable->FindRow<FEPWeaponDataRow>(FName("ShotGun"), FString("ShotGunDataContext"));

			HandGunData = SetData(HandGunDataRow);
			RifleData = SetData(RifleDataRow);
			ShotGunData = SetData(ShotGunDataRow);

			UE_LOG(LogTemp, Warning, TEXT("DataLoad Succeed"));


		}
	}





	// ...
	
}





void UEPWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//AR->SetActorTransform(Player->GetMesh()->GetSocketTransform(FName("hand_r_ability_socket")));
	
	// ...
}

void UEPWeaponComponent::GunFire()
{
	if (!Player)
	{
		return;
	}

	ECharacterState State = Player->CharacterState;


	if (State == ECharacterState::Pistol)
	{
		GunFireLogic(HandGunData);
	}
	else if (State == ECharacterState::Rifle)
	{
		GunFireLogic(RifleData);
		UE_LOG(LogTemp, Warning, TEXT("Rifle "));

		//Rifles.FireGun();
	}
	else if (State == ECharacterState::Shotgun)
	{
		GunFireLogic(ShotGunData);
		//Rifles.FireGun();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("default "));
		return;
	}
	
	
	//탄환 생성
	//MuzzleEffect->rota
	


	
}

void UEPWeaponComponent::GunFireLogic(FEPWeaponData& Data)
{

	if (FireReady)
	{
		Player->GetWorldTimerManager().SetTimer(
			FireDelayTimerHandle,
			this,
			&UEPWeaponComponent::SetFireReady,
			Data.FireDelay,
			false
		);
		FireReady = false;
		if (Data.Ammo <= 0)
		{
			ReloadLogic(Data);
			return;
		}
		//파티클
		Data.Ammo--;
		if (WeaponMesh)
		{
			FTransform MuzzleTransform = WeaponMesh->GetSocketTransform(FName("Muzzle"), RTS_World);
			GetWorld()->SpawnActor<AActor>(RifleBullet, MuzzleTransform);
		}
		UE_LOG(LogTemp, Warning, TEXT("Fire! %d / %d"), Data.Ammo, Data.MaxAmmo);
	}
	else
	{
		return;
	}
}

void UEPWeaponComponent::Reload()
{
	ECharacterState State = Player->CharacterState;


	if (State == ECharacterState::Pistol)
	{
		ReloadLogic(HandGunData);
	}
	else if (State == ECharacterState::Rifle)
	{
		ReloadLogic(RifleData);
	}
	else if (State == ECharacterState::Shotgun)
	{
		ReloadLogic(ShotGunData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("default "));
		return;
	}
}

void UEPWeaponComponent::ReloadLogic(FEPWeaponData& Data)
{
	
	//UEPInventoryComponent* Inventory = Player->InventoryComponent
	if (Data.Ammo < 0)
	{
		Data.Ammo = 0;
	}
	int32 NeedAmmo = Data.MaxAmmo - Data.Ammo;
	switch (Player->CharacterState)
	{
	case ECharacterState::Unarmed:
		break;
	case ECharacterState::Pistol:
		if (Player->InventoryComponent->HandGunAmmo <= 0)
		{
			return;
		}
		else if (NeedAmmo > Player->InventoryComponent->HandGunAmmo)
		{
			Data.Ammo = Player->InventoryComponent->HandGunAmmo;
			Player->InventoryComponent->HandGunAmmo = 0;
		}
		else
		{
			Data.Ammo = Data.MaxAmmo;
			Player->InventoryComponent->HandGunAmmo -= NeedAmmo;
		}
		
		break;
	case ECharacterState::Rifle:
		if (Player->InventoryComponent->RifleAmmo <= 0)
		{
			return;
		}
		else if (NeedAmmo > Player->InventoryComponent->RifleAmmo)
		{
			Data.Ammo = Player->InventoryComponent->RifleAmmo;
			Player->InventoryComponent->RifleAmmo = 0;
		}
		else
		{
			Data.Ammo = Data.MaxAmmo;
			Player->InventoryComponent->RifleAmmo -= NeedAmmo;
		}

		break;
	case ECharacterState::Shotgun:
		if (Player->InventoryComponent->ShotGunAmmo <= 0)
		{
			return;
		}
		else if (NeedAmmo > Player->InventoryComponent->ShotGunAmmo)
		{
			Data.Ammo = Player->InventoryComponent->ShotGunAmmo;
			Player->InventoryComponent->ShotGunAmmo = 0;
		}
		else
		{
			Data.Ammo = Data.MaxAmmo;
			Player->InventoryComponent->ShotGunAmmo -= NeedAmmo;
		}
		break;
	default:
		break;
	}

}


void UEPWeaponComponent::SetFireReady()
{
	FireReady = true;
}

FEPWeaponData UEPWeaponComponent::SetData(FEPWeaponDataRow* DataRow)
{
	FEPWeaponData SettingData;
	SettingData.Ammo = DataRow->Ammo;
	SettingData.MaxAmmo = DataRow->MaxAmmo;
	SettingData.Damage = DataRow->Damage;
	SettingData.FireDelay = DataRow->FireDelay;
	SettingData.WeaponType = DataRow->WeaponType;

	return SettingData;
}


