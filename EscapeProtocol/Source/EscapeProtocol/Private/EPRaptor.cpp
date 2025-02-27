#include "EPRaptor.h"

AEPRaptor::AEPRaptor()
{
	PrimaryActorTick.bCanEverTick = false;


	MaxHealth = 200.0f;
	AttackDamage = 20.0f;
	Health = MaxHealth;
	patrolRadius = 1000.0f;
	PatrolSpeed = 400.0f;
	ChaseSpeed = 700.0f;

}
