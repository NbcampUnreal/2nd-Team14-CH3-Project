// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameState.h"
AEPGameState::AEPGameState()
{
	Score = 0;
	CurrentLevelIndex = 0;
	MaxLevel = 2;
}

void AEPGameState::BeginPlay()
{

	Super::BeginPlay();

	StartLevel();

}

int32 AEPGameState::GetScore() const
{
	return Score;
}

void AEPGameState::AddScore(int32 Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("Add Score"));
}

void AEPGameState::StartLevel()
{
}

void AEPGameState::EndLevel()
{
}

