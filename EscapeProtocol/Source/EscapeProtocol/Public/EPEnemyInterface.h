#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EPEnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEPEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class ESCAPEPROTOCOL_API IEPEnemyInterface
{
	GENERATED_BODY()

public:
	virtual float GetHealth() const = 0;
	virtual float GetAttackerPower() const = 0;
	virtual void Attack() = 0;
	virtual void DropLoot() = 0;


};
