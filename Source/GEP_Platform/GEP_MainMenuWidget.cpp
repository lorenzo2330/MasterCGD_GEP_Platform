#include "GEP_MainMenuWidget.h"

#include "GEP_ConfirmWidget.h"
#include "GEP_Platform.h"
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
		UE_LOG(LogGEP_Platform, Warning, TEXT("Error: couldn't find saved data"));
		ButtonContinue->SetIsEnabled(false);
	}
	
	EnterUIMode();
}

void UGEP_MainMenuWidget::OnNewGameClicked()
{
	if (ActiveConfirm) { return; }

	if (GameLevelName == NAME_None)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("GameLevelName not set in WBP_MainMenu Class Defaults"));
		return;
	}

	UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
	if (!Save)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("OnNewGameClicked: SaveSubsystem not found"));
		return;
	}

	if (!Save->HasSave())
	{
		StartNewGameConfirmed();
		return;
	}

	if (!ConfirmWidgetClass)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("ConfirmWidgetClass not set in WBP_MainMenu Class Defaults"));
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("OnNewGameClicked: no owning player"));
		return;
	}

	ActiveConfirm = CreateWidget<UGEP_ConfirmWidget>(PC, ConfirmWidgetClass);
	if (!ActiveConfirm)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("OnNewGameClicked: CreateWidget failed for ConfirmWidgetClass"));
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
		UE_LOG(LogGEP_Platform, Error, TEXT("GameLevelName not set in WBP_MainMenu Class Defaults."));
		return;
	}

	UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
	if (!Save)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("SaveSubsystem not found: Continue disabled."));
		ButtonContinue->SetIsEnabled(false);
		return;
	}

	if (!Save->RequestContinue())
	{
		UE_LOG(LogGEP_Platform, Warning, TEXT("RequestContinue() failed: save missing, corrupted or wrong version. Continue disabled."));
		ButtonContinue->SetIsEnabled(false);
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameLevelName, true);
}

void UGEP_MainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UGEP_MainMenuWidget::StartNewGameConfirmed()
{
	UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
	if (!Save)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("SaveSubsystem not found: cannot start a new game."));
		return;
	}

	Save->DeleteSave();
	Save->StartNewGame();
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
