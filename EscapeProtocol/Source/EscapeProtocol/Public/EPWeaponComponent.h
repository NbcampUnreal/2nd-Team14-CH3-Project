// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPCharacter.h"
#include "EPWeaponDataRow.h"
#include "EPWeaponData.h"
#include "GameFramework/Actor.h"

#include "EPWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEPROTOCOL_API UEPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEPWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	AEPCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* WeaponMesh;

	FTransform SocketPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<AActor> RifleBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire");
	bool FireReady;

	FTimerHandle FireDelayTimerHandle;

	FEPWeaponDataRow* HandGunDataRow;
	FEPWeaponDataRow* RifleDataRow;
	FEPWeaponDataRow* ShotGunDataRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Data");
	FEPWeaponData HandGunData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Data");
	FEPWeaponData RifleData;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Weapon|Data");
	FEPWeaponData ShotGunData;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void GunFire();

	UFUNCTION()
	void GunFireLogic(FEPWeaponData& Data);

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void ReloadLogic(FEPWeaponData& Data);

	UFUNCTION()
	void SetFireReady();

	FEPWeaponData SetData(FEPWeaponDataRow* DataRow);
};
