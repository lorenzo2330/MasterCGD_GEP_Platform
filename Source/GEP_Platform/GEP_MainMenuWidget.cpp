// Fill out your copyright notice in the Description page of Project Settings.


#include "GEP_MainMenuWidget.h"

#include "GEP_ConfirmWidget.h"
#include "GEP_SaveSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGEP_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ButtonNewGame->OnClicked.AddDynamic(this, &UGEP_MainMenuWidget::OnNewGameClicked);
	ButtonContinue->OnClicked.AddDynamic(this, &UGEP_MainMenuWidget::OnContinueClicked);
	ButtonQuit->OnClicked.AddDynamic(this, &UGEP_MainMenuWidget::OnQuitClicked);
	
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		ButtonContinue->SetIsEnabled(Save->HasSave());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: couldn't find saved data"));
		ButtonContinue->SetIsEnabled(false);
	}
	
	EnterUIMode();
}

void UGEP_MainMenuWidget::OnNewGameClicked()
{
	if (ActiveConfirm) { return; }

	if (GameLevelName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("GameLevelName not set in WBP_MainMenu Class Defaults"));
		return;
	}

	UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
	if (!Save)
	{
		UE_LOG(LogTemp, Error, TEXT("OnNewGameClicked: SaveSubsystem not found"));
		return;
	}

	if (!Save->HasSave())
	{
		StartNewGameConfirmed();
		return;
	}

	if (!ConfirmWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ConfirmWidgetClass not set in WBP_MainMenu Class Defaults"));
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("OnNewGameClicked: no owning player"));
		return;
	}

	ActiveConfirm = CreateWidget<UGEP_ConfirmWidget>(PC, ConfirmWidgetClass);
	if (!ActiveConfirm)
	{
		UE_LOG(LogTemp, Error, TEXT("OnNewGameClicked: CreateWidget failed for ConfirmWidgetClass"));
		return;
	}

	ActiveConfirm->OnConfirmed.AddDynamic(this, &UGEP_MainMenuWidget::StartNewGameConfirmed);
	ActiveConfirm->OnCancelled.AddDynamic(this, &UGEP_MainMenuWidget::CloseConfirm);
	ActiveConfirm->AddToViewport(10);
}

void UGEP_MainMenuWidget::OnContinueClicked()
{
	if (GameLevelName == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: GameLevelName is None (need to be set in Class Defaults)"));
		return;
	}
	
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		if (Save->RequestContinue())
		{
			UGameplayStatics::OpenLevel(GetWorld(), GameLevelName, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error: couldn't load the level"));
			ButtonContinue->SetIsEnabled(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: couldn't find saved data"));
		ButtonContinue->SetIsEnabled(false);
	}
}

void UGEP_MainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UGEP_MainMenuWidget::StartNewGameConfirmed()
{
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this))
	{
		Save->DeleteSave();
		Save->StartNewGame();
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), GameLevelName, true);
}

void UGEP_MainMenuWidget::CloseConfirm()
{
	if (ActiveConfirm)
	{
		ActiveConfirm->RemoveFromParent();
	}

	ActiveConfirm = nullptr;
	
	EnterUIMode();
}
