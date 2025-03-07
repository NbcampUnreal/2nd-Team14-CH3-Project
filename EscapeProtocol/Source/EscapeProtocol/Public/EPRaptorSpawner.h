#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPRaptorSpawner.generated.h"

UCLASS()
class ESCAPEPROTOCOL_API AEPRaptorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPRaptorSpawner();

protected:
	virtual void BeginPlay() override;

	// 타이머 핸들 선언
	FTimerHandle SpawnTimerHandle;

	// 지정한 위치에 스폰하는 함수
	void SpawnRaptor();

public:	
	// 스폰할 AEPRaptor의 클래스(블루프린트에서 지정 가능)
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AEPRaptor> RaptorClass;

	// 스폰 위치와 회전값을 지정할 수 있는 변수
	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnTime;
};
