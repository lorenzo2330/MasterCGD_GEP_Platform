#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GEP_PlatformGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameFlowState : uint8
{
	Playing,
	Paused,
	GameOver,
	Victory
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFlowStateChanged, EGameFlowState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyCountChanged, int32, Alive, int32, Total);

UCLASS(abstract)
class GEP_PLATFORM_API AGEP_PlatformGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGEP_PlatformGameMode();

	UFUNCTION(BlueprintPure, Category = "Game Flow")
	EGameFlowState GetFlowState() const { return FlowState; }

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void TogglePause();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void HandlePlayerDeath();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void TriggerVictory();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void RestartCurrentLevel();

	void SetRespawnTransform(const FTransform& NewTransform) { RespawnTransform = NewTransform; }
	const FTransform& GetRespawnTransform() const { return RespawnTransform; }

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void RegisterEnemy();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void NotifyEnemyDefeated();
	
	void SaveProgress(int32 MaxHP, int32 Coins) const;

	UPROPERTY(BlueprintAssignable, Category = "Game Flow")
	FOnGameFlowStateChanged OnStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Game Flow")
	FOnEnemyCountChanged OnEnemyCountChanged;
	
	/** Level opened by "Return to main menu". */
	UPROPERTY(EditDefaultsOnly, Category = "Game Flow")
	FName MainMenuLevelName;

	/** Resumes the world, restores input and opens the main menu level. */
	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void ReturnToMainMenu();
	
	int32 GetAliveEnemies() const { return AliveEnemies; }
	int32 GetTotalEnemies() const { return TotalEnemies; }

private:
	void SetFlowState(EGameFlowState NewState);

	EGameFlowState FlowState = EGameFlowState::Playing;
	FTransform RespawnTransform;
	
	int32 AliveEnemies;
	int32 TotalEnemies;
};