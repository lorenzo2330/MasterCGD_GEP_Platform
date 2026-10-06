// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GEP_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GEP_PLATFORM_API UGEP_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	//Incrementare ad ogni cambiamento, in modo che possa scartare salvataggi vecchi
	static constexpr int32 CurrentVersion = 1;

	UPROPERTY() int32 SaveVersion = CurrentVersion;		//Ultima versione dei dati di salvataggio
	UPROPERTY() FTransform RespawnTransform;			//Coinciderà con l'ultimo checkpoint attivato
	UPROPERTY() int32 MaxHP = 3;						//Numero massimo di HP (può variare per scelte di design)
	UPROPERTY() int32 Coins = 0;						//Valore in monete (potenzialmente != da CollectedCoins.Num() perchè Coins ha un Value)
	UPROPERTY() TSet<FName> CollectedCoins;				//Quali monete ha raccolto (in modo da evitare che respawnino)
	
};



