// Fill out your copyright notice in the Description page of Project Settings.


#include "GEP_PauseMenuWidget.h"

#include "GEP_PlatformGameMode.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UGEP_PauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ButtonResume->OnClicked.AddDynamic(this, &UGEP_PauseMenuWidget::OnResumeClicked);
	ButtonMainMenu->OnClicked.AddDynamic(this, &UGEP_PauseMenuWidget::OnMainMenuClicked);
	ButtonQuit->OnClicked.AddDynamic(this, &UGEP_PauseMenuWidget::OnQuitClicked);
	
	EnterUIMode();
}

FReply UGEP_PauseMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (CloseKeys.Contains(InKeyEvent.GetKey()))
	{
		AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	
		if (GM)
		{
			GM->TogglePause();
			
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UGEP_PauseMenuWidget::OnResumeClicked()
{
	AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	
	if (GM)
	{
		GM->TogglePause();
	}
}

void UGEP_PauseMenuWidget::OnMainMenuClicked()
{
	AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	
	if (GM)
	{
		GM->ReturnToMainMenu();
	}
}

void UGEP_PauseMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
