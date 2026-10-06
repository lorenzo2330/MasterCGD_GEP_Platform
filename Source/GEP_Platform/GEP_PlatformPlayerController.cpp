// Copyright Epic Games, Inc. All Rights Reserved.


#include "GEP_PlatformPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "GEP_Platform.h"
#include "GEP_PlatformGameMode.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AGEP_PlatformPlayerController::OnPausePressed()
{
	AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	
	if (GM)
	{
		GM->TogglePause();
	}
}

void AGEP_PlatformPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (IsLocalPlayerController() && ShouldUseTouchControls())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogGEP_Platform, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AGEP_PlatformPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	
	if (!EIC)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find enhanced input component for player controller"));
		return;
	}

	if (!PauseAction)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseAction not set in the PlayerController Blueprint"));
		return;
	}
	EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &AGEP_PlatformPlayerController::OnPausePressed);
}

bool AGEP_PlatformPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
