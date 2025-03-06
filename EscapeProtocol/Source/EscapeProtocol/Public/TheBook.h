// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheBook.generated.h"

class USphereComponent;

UCLASS()
class ESCAPEPROTOCOL_API ATheBook : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATheBook();

	

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Book|Component")
    USceneComponent* Scene;
    // 충돌 컴포넌트 (플레이어 진입 범위 감지용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Book|Component")
    USphereComponent* Collision;
    // 아이템 시각 표현용 스태틱 메시
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Book|Component")
    UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void ActivateItem(AActor* Activator);
	void DestroyItem();
};
