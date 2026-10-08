// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEP_PlatformGameMode.h"

#include "GEP_Platform.h"
#include "GEP_PlatformPlayerController.h"
#include "GEP_PlayerState.h"
#include "GEP_SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGEP_PlatformGameMode::AGEP_PlatformGameMode(){}

void AGEP_PlatformGameMode::ReturnToMainMenu()
{
	if (MainMenuLevelName.IsNone())
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("MainMenuLevelName not set in the GameMode Blueprint"));
		return;
	}
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
	
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
	
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
	UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
	if (Save)
	{
		Save->DeleteSave();
	}
	else
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("SaveSubsystem not found: save file not deleted on victory."));
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
	
	UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
	if (!Save)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("SaveSubsystem not found: restarting without save/session reset."));
	}
	else if (!Save->RequestContinue())
	{
		Save->StartNewGame();
	}

	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void AGEP_PlatformGameMode::DebugVictory()
{
	TriggerVictory();
}

void AGEP_PlatformGameMode::DebugDeleteSave() 
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
		if (!Save->SaveAtCheckpoint(RespawnTransform, MaxHP, Coins))
		{
			UE_LOG(LogGEP_Platform, Error, TEXT("Impossible to save"));
		}
	}
	else
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("SaveSubsystem not found: restarting without save/session reset"));
	}
}
