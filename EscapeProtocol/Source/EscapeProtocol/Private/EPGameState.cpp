// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AudioComponent.h"

AEPGameState::AEPGameState()
{
	Score = 0;
	CurrentLevelIndex = 0;
	MaxLevel = 2;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;

}

void AEPGameState::PlayBackgroundMusic()
{
//	// 배경음악 Sound Cue가 지정되어 있다면
	if (BackgroundMusic)
	{
//		// 배경음악을 재생
		AudioComponent->SetSound(BackgroundMusic);
		AudioComponent->Play();
	}
}

void AEPGameState::StopBackgroundMusic()
{
	// 배경음악을 멈추기
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
}


void AEPGameState::BeginPlay()
{

	Super::BeginPlay();

	StartLevel();
	EPBook = false;
	PlayBackgroundMusic();


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

bool AEPGameState::GetEPBook()
{
	return EPBook;
}

void AEPGameState::EndGame()
{





	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	// 게임 종료 (메인 메뉴로 이동 또는 종료)
	//UGameplayStatics::OpenLevel(this, FName("JSTestMap")); // 메인 메뉴로 이동하거나 게임 종료
}
