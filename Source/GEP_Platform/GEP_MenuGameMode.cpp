// Fill out your copyright notice in the Description page of Project Settings.


#include "GEP_MenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AGEP_MenuGameMode::AGEP_MenuGameMode()
{
	DefaultPawnClass = nullptr;
}

void AGEP_MenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	
	if (MainMenuWidgetClass && PC)
	{
		CreateWidget<UUserWidget>(PC, MainMenuWidgetClass)->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error, MainMenuWidgetClass not set"));
	}
	
	
}
