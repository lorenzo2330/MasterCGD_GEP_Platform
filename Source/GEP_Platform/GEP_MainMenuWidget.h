// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEP_MenuWidgetBase.h"
#include "GEP_MainMenuWidget.generated.h"

class UGEP_ConfirmWidget;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class GEP_PLATFORM_API UGEP_MainMenuWidget : public UGEP_MenuWidgetBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
	//I nomi nel WBP devono essere esattamente questi
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonNewGame;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonContinue;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonQuit;
	
	UPROPERTY(EditDefaultsOnly) FName GameLevelName;
	
	UFUNCTION() void OnNewGameClicked();
	UFUNCTION() void OnContinueClicked();
	UFUNCTION() void OnQuitClicked();
	
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGEP_ConfirmWidget> ConfirmWidgetClass;
	UPROPERTY() TObjectPtr<UGEP_ConfirmWidget> ActiveConfirm;
	UFUNCTION() void StartNewGameConfirmed(); 
	UFUNCTION() void CloseConfirm();
};
