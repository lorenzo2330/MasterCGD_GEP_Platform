#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/GEP_PlatformGameMode.h"
#include "GEP_UserWidget.generated.h"

class AGEP_Boss;
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
	TObjectPtr<UTextBlock> TextBossHealth;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextCoins;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextNumEnemies;

private:
	//Si lega a Health e PlayerState se disponibili (altrimenti attende il possesso del pawn)
	void TryBind();
	void TryBindBoss(AGEP_Boss* Boss);
	void Unbind();

	UFUNCTION()
	void HandleHealthChanged(int32 CurrentHP, int32 MaxHP) const;
	
	UFUNCTION()
	void HandleBossHealthChanged(int32 CurrentHP, int32 MaxHP) const;

	UFUNCTION()
	void HandleCoinsChanged(int32 TotalCoins) const;
	
	UFUNCTION()
	void HandleNumEnemyChanged(int32 CurrentNumEnemy, int32 TotalNumEnemy) const;

	UFUNCTION()
	void HandlePawnChanged(APawn* OldPawn, APawn* NewPawn);

	void UpdateCoinsText(const AGEP_PlayerState* PS) const;
	void UpdateNumEnemyText(const AGEP_PlatformGameMode* GM) const;

	TWeakObjectPtr<UHealth> BoundHealth;
	TWeakObjectPtr<UHealth> BoundBossHealth;
	TWeakObjectPtr<AGEP_PlayerState> BoundPlayerState;
	TWeakObjectPtr<AGEP_PlatformGameMode> BoundGameMode;
};