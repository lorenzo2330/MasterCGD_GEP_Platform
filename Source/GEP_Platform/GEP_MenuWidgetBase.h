// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GEP_MenuWidgetBase.generated.h"

UCLASS(Abstract)
class GEP_PLATFORM_API UGEP_MenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGEP_MenuWidgetBase(const FObjectInitializer& ObjectInitializer);
	
protected:
	/** Cursor on, UI-only input, focus on this widget. */
	void EnterUIMode();

	/** Cursor off, game-only input. */
	void EnterGameMode() const;
};
