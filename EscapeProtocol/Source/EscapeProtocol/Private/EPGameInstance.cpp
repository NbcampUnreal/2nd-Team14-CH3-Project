// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameInstance.h"

UEPGameInstance::UEPGameInstance()
{

	TotalKilledEnemy = 0;
	CurrentLevelIndex = 0;

}

void UEPGameInstance::AddToTotalKilledEnemy(int32 Amount)
{

	UE_LOG(LogTemp, Warning, TEXT("Total Killed Enemy Updated: %d"), TotalKilledEnemy);


}