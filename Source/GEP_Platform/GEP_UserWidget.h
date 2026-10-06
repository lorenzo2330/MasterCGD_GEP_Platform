#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GEP_UserWidget.generated.h"

class UTextBlock;
class UHealth;
class AGEP_PlayerState;

UCLASS(Abstract)
class GEP_PLATFORM_API UGEP_UserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//I nomi dei TextBlock nel WBP_HUD devono essere esattamente questi
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextHealth;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextCoins;

private:
	//Si lega a Health e PlayerState se disponibili (altrimenti attende il possesso del pawn)
	void TryBind();
	void Unbind();

	UFUNCTION()
	void HandleHealthChanged(int32 CurrentHP, int32 MaxHP) const;

	UFUNCTION()
	void HandleCoinsChanged(int32 TotalCoins) const;

	UFUNCTION()
	void HandlePawnChanged(APawn* OldPawn, APawn* NewPawn);

	void UpdateCoinsText(const AGEP_PlayerState* PS) const;

	TWeakObjectPtr<UHealth> BoundHealth;
	TWeakObjectPtr<AGEP_PlayerState> BoundPlayerState;
};