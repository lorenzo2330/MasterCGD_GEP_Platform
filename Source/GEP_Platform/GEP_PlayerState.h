#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GEP_PlayerState.generated.h"

//Broadcast per il numero di monete (usato dall'HUD)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsChanged, int32, TotalCoins);
//Per il bonus vite (per feedback audio/VFX).
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinBonusGranted);

UCLASS()
class GEP_PLATFORM_API AGEP_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void AddCoins(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SetCoins(int32 NewTotal);

	UFUNCTION(BlueprintPure, Category = "Coins")
	int32 GetCoins() const { return Coins; }

	UFUNCTION(BlueprintPure, Category = "Coins")
	int32 GetCoinsTowardBonus() const;

	UPROPERTY(BlueprintAssignable, Category = "Coins")
	FOnCoinsChanged OnCoinsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Coins")
	FOnCoinBonusGranted OnBonusGranted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coins", meta = (ClampMin = "1"))
	int32 CoinsPerBonus = 10;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Coins")
	int32 Coins = 0;
};