// GEP_EndScreenWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GEP_EndScreenWidget.generated.h"

class UButton;

UCLASS(Abstract)
class GEP_PLATFORM_API UGEP_EndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	//Il nome nel Blueprint deve essere esattamente questo
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Restart;

private:
	UFUNCTION()
	void OnRestartClicked();
};