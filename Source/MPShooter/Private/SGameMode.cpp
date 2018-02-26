// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "TimerManager.h"
#include "SHealthComponent.h"
#include "SGameState.h"
#include "SPlayerState.h"

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;

	GameStateClass = ASGameMode::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}


void ASGameMode::StartWave()
{
	WaveCount++;
	AmountOfBotsToSpawn = WaveCount * 2;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}


void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
}


void ASGameMode::PrepareToNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);
}


void ASGameMode::CheckWaveState()
{
	bool bIsPreparingFoNextWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (AmountOfBotsToSpawn > 0 || bIsPreparingFoNextWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		auto HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareToNextWave();
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* PlayerPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(PlayerPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				// any player still alive
				return;
			}
		}
	}

	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("Game over, players died!"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}


void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareToNextWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();
}


void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	AmountOfBotsToSpawn--;

	if (AmountOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}
