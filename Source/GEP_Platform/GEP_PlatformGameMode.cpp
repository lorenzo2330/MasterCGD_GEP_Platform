// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEP_PlatformGameMode.h"
#include "Kismet/GameplayStatics.h"

AGEP_PlatformGameMode::AGEP_PlatformGameMode()
{
	// stub
}

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
	
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void AGEP_PlatformGameMode::DebugVictory()
{
	TriggerVictory();
}