// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GEP_MenuGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class GEP_PLATFORM_API AGEP_MenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGEP_MenuGameMode();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
private:
	
	
};
