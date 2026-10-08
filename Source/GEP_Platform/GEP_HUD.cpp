#include "GEP_HUD.h"

#include "Blueprint/UserWidget.h"
#include "GEP_Platform.h"
#include "GEP_PlatformGameMode.h"
#include "GEP_UserWidget.h"

void AGEP_HUD::BeginPlay()
{
	Super::BeginPlay();

	AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	if (GM)
	{
		GM->OnStateChanged.AddDynamic(this, &AGEP_HUD::HandleStateChanged);
	}
	else
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("GameMode is not AGEP_PlatformGameMode: check World Settings of this map."));
	}

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("AGEP_HUD has no owning PlayerController."));
		return;
	}

	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);

	if (!WidgetClass)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("WidgetClass not set: assign WBP_HUD in BP_GEP_HUD."));
		return;
	}

	Widget = CreateWidget<UGEP_UserWidget>(PC, WidgetClass);
	if (!Widget)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("CreateWidget failed for the HUD WidgetClass."));
		return;
	}

	Widget->AddToViewport();
}

void AGEP_HUD::HandleStateChanged(EGameFlowState NewState)
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) { return; }

	
	if (ActiveOverlay)
	{
		ActiveOverlay->RemoveFromParent();
		ActiveOverlay = nullptr;
	}

	TSubclassOf<UUserWidget> OverlayClass;
	if (NewState == EGameFlowState::GameOver) 
	{
		OverlayClass = GameOverWidgetClass;
	}
	else if (NewState == EGameFlowState::Victory)
	{
		OverlayClass = VictoryWidgetClass;
	}
	else if (NewState == EGameFlowState::Paused)
	{
		OverlayClass = PausedWidgetClass;
	}

	if (!OverlayClass && NewState != EGameFlowState::Playing)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("No overlay widget class for this state: set GameOver/Victory/Paused WidgetClass in BP_GEP_HUD."));
		return;
	}
	
	if (OverlayClass)
	{
		ActiveOverlay = CreateWidget<UUserWidget>(PC, OverlayClass);
		
		if (!ActiveOverlay)
		{
			UE_LOG(LogGEP_Platform, Warning, TEXT("Failed to create Overlay Widget"));
			return;
		}
		
		ActiveOverlay->AddToViewport(10);

		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(ActiveOverlay->TakeWidget());
		PC->SetInputMode(Mode);
		PC->SetShowMouseCursor(true);
	}
	else if (NewState == EGameFlowState::Playing)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}