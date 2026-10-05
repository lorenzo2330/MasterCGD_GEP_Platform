#pragma once

#include "GEP_PlatformGameMode.h" 
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GEP_HUD.generated.h"

class UGEP_UserWidget;

UCLASS()
class GEP_PLATFORM_API AGEP_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UGEP_UserWidget> WidgetClass;

	UPROPERTY()
	TObjectPtr<UGEP_UserWidget> Widget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveOverlay;

	UFUNCTION()
	void HandleStateChanged(EGameFlowState NewState);
};