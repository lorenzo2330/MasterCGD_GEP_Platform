#include "GEP_HUD.h"
#include "GEP_UserWidget.h"

void AGEP_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = GetOwningPlayerController())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}

	if (WidgetClass && GetOwningPlayerController())
	{
		Widget = CreateWidget<UGEP_UserWidget>(GetOwningPlayerController(), WidgetClass);
		if (Widget) { Widget->AddToViewport(); }
	}
	
	if (AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>())
	{
		GM->OnStateChanged.AddDynamic(this, &AGEP_HUD::HandleStateChanged);
	}
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

	if (OverlayClass)
	{
		ActiveOverlay = CreateWidget<UUserWidget>(PC, OverlayClass);
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