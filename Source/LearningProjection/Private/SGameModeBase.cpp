// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributesComponent.h"
#include "EngineUtils.h"
#include <../SCharacter.h>


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su. SpawnBots"), true, TEXT("Enable spatning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	//continuous timer to spawn in more bots.
	//Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via Cvar 'CVarspawnBots'"));
		return;
	}
	int32 NumOfAliveBot = 0;
	for (TActorIterator<ASAICharacter> iter(GetWorld()); iter; ++iter)
	{
		ASAICharacter* Bot = *iter;

		USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(Bot);
		//if (AttributeComp && AttributeComp->IsAlive())
		if (AttributeComp)
		{
			NumOfAliveBot++;
		}
	}

	int32 MaxBotCount = 10;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBot >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}


void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Eqs Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		//UE_LOG(LogTemp, Warning, TEXT("Create Bot%d"), NumOfAliveBot);
	}
}

void ASGameModeBase::RespawnPlayerTimerElapsed(AController* Contronller)
{
	if (ensure(Contronller))
	{
		Contronller->UnPossess();

		RestartPlayer(Contronller);
	}
}
void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_SpawnDelay;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "RespawnPlayerTimerElapsed", Player->GetController());//以函数名设置定时器

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnDelay, TimerDelegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer:%s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

/* 控制台Exec函数 */
void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> iter(GetWorld()); iter; ++iter)
	{
		ASAICharacter* Bot = *iter;

		USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(Bot);
		//if (AttributeComp && AttributeComp->IsAlive())
		if (AttributeComp)
		{
			AttributeComp->KillSelf(this); //@fix me: pass in player? for kill credits
		}
	}
}