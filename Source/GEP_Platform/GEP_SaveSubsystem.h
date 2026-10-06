// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GEP_SaveSubsystem.generated.h"

class UGEP_SaveGame;
/**
 * 
 */
UCLASS()
class GEP_PLATFORM_API UGEP_SaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static constexpr const TCHAR* SlotName = TEXT("MainSlot");
	static UGEP_SaveSubsystem* Get(const UObject* WorldContext);

	//Controlla se ci sono salvataggi sul disco
	UFUNCTION(BlueprintCallable) bool HasSave() const;
	
	//Rimuove il salvataggio dal disco
	UFUNCTION(BlueprintCallable) void DeleteSave();

	//Svuota PendingLoad e SessionCollectedCoins per una nuova partita da zero
	UFUNCTION(BlueprintCallable) void StartNewGame();

	//Carica nella sessione attuale gli ultimi dati salvati nel disco
	UFUNCTION(BlueprintCallable) bool RequestContinue();

	//Utilizzata quando si carica un nuovo livello, consuma i dati della fine della sessione precedente (la fine del livello)
	UGEP_SaveGame* ConsumePendingLoad();

	//Salva uno snapshot del gioco sul disco
	bool SaveAtCheckpoint(const FTransform& RespawnTransform, int32 MaxHP, int32 Coins) const;

	void NotifyCoinCollected(FName CoinName);
	bool IsCoinCollected(FName CoinName) const;

private:
	UPROPERTY() TObjectPtr<UGEP_SaveGame> PendingLoad;
	TSet<FName> SessionCollectedCoins;
};
