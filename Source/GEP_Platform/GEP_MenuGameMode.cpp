
#include "GEP_MenuGameMode.h"

#include "GEP_Platform.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AGEP_MenuGameMode::AGEP_MenuGameMode()
{
	DefaultPawnClass = nullptr;
}

void AGEP_MenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("MainMenuWidgetClass not set: assign WBP_MainMenu in BP_MenuGameMode."));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("PlayerController 0 is null: cannot create the main menu widget."));
		return;
	}

	UUserWidget* ActiveWidget = CreateWidget<UUserWidget>(PC, MainMenuWidgetClass);
	if (!ActiveWidget)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("CreateWidget failed for MainMenuWidgetClass."));
		return;
	}

	ActiveWidget->AddToViewport();
}
