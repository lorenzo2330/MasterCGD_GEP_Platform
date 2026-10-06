// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEP_PlatformGameMode.h"

#include "GEP_PlayerState.h"
#include "GEP_SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGEP_PlatformGameMode::AGEP_PlatformGameMode(){}

void AGEP_PlatformGameMode::SetFlowState(EGameFlowState NewState)
{
	if (FlowState == NewState || FlowState == EGameFlowState::GameOver || FlowState == EGameFlowState::Victory) { return; }

	FlowState = NewState;
	UGameplayStatics::SetGamePaused(this, NewState != EGameFlowState::Playing);
	OnStateChanged.Broadcast(NewState);
}

void AGEP_PlatformGameMode::TogglePause()
{
	if (FlowState == EGameFlowState::Playing)
	{
		SetFlowState(EGameFlowState::Paused);
	}
	else if (FlowState == EGameFlowState::Paused)
	{
		SetFlowState(EGameFlowState::Playing);
	}
}

void AGEP_PlatformGameMode::HandlePlayerDeath()
{
	SetFlowState(EGameFlowState::GameOver);
}

void AGEP_PlatformGameMode::TriggerVictory()
{
	// La vittoria chiude la partita: nessun "Continua"
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		Save->DeleteSave();
	}
	SetFlowState(EGameFlowState::Victory);
}

void AGEP_PlatformGameMode::RestartCurrentLevel()
{
	UGameplayStatics::SetGamePaused(this, false);
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
	
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		if (!Save->RequestContinue()) { Save->StartNewGame(); }
	}

	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void AGEP_PlatformGameMode::DebugVictory()
{
	TriggerVictory();
}

void AGEP_PlatformGameMode::DebugDeleteSave()   // dichiarata UFUNCTION(Exec) nel .h
{
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		Save->DeleteSave();
		Save->StartNewGame();
	}
}

void AGEP_PlatformGameMode::SaveProgress(int32 MaxHP, int32 Coins) const
{
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		Save->SaveAtCheckpoint(RespawnTransform, MaxHP, Coins);
	}
}
