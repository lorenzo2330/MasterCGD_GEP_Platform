// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "GEP_MenuWidgetBase.h"
#include "GEP_PauseMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS(Abstract)
class GEP_PLATFORM_API UGEP_PauseMenuWidget : public UGEP_MenuWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//I nomi nel WBP devono coincidere
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonResume;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonMainMenu;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonQuit;

	/** Keys that close the pause menu (set in WBP: Escape, plus P for PIE tests). */
	UPROPERTY(EditDefaultsOnly, Category = "Pause") TArray<FKey> CloseKeys;

	UFUNCTION() void OnResumeClicked();
	UFUNCTION() void OnMainMenuClicked();
	UFUNCTION() void OnQuitClicked();
	
};
