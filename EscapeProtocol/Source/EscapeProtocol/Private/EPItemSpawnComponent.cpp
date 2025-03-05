// Fill out your copyright notice in the Description page of Project Settings.


#include "EPItemSpawnComponent.h"

// Sets default values for this component's properties
UEPItemSpawnComponent::UEPItemSpawnComponent()
{

	
	
}

void UEPItemSpawnComponent::BeginPlay()
{
	Super::BeginPlay();
	UGameInstance* Instance = GetWorld()->GetGameInstance();
	if (Instance)
	{
		GameInstance = Cast<UEPGameInstance>(Instance);
	}
}

FEPItemSpawnRow* UEPItemSpawnComponent::GetRandomItem() const
{
	TArray<FEPItemSpawnRow*> AllRow;
	const FString ContextString(TEXT("ItemSpawnContext"));
	GameInstance->SpawnItemDataTable->GetAllRows(ContextString, AllRow);

	if(AllRow.IsEmpty())
		return nullptr;

	float TotalChance = 0.0f; // 초기화
	for (const FEPItemSpawnRow* Row : AllRow) // AllRows 배열의 각 Row를 순회
	{
		if (Row) // Row가 유효한지 확인
		{
			TotalChance += Row->SpawnChance; // SpawnChance 값을 TotalChance에 더하기
		}
	}

	// 3) 0 ~ TotalChance 사이 랜덤 값
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	// 4) 누적 확률로 아이템 선택
	for (FEPItemSpawnRow* Row : AllRow)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			if (Row->ItemName == FName("VoidItem"))
			{
				return nullptr;
			}
			return Row;
		}
	}

	return nullptr;
	


	
}

void UEPItemSpawnComponent::SpawnItem(FVector Location)
{
	if (FEPItemSpawnRow* SeletedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SeletedRow->ItemClass.Get())
		{
			if (ActualClass)
			{
				GetWorld()->SpawnActor<AActor>(ActualClass, Location, FRotator::ZeroRotator);
			}
		}
	}
}


// Called when the game starts


