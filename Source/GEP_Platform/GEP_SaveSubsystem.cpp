// Fill out your copyright notice in the Description page of Project Settings.


#include "GEP_SaveSubsystem.h"

#include "GEP_SaveGame.h"
#include "Kismet/GameplayStatics.h"

UGEP_SaveSubsystem* UGEP_SaveSubsystem::Get(const UObject* WorldContext)
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContext);
	return GI ? GI->GetSubsystem<UGEP_SaveSubsystem>() : nullptr;
}

bool UGEP_SaveSubsystem::HasSave() const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void UGEP_SaveSubsystem::DeleteSave()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
		PendingLoad	= nullptr;
		SessionCollectedCoins.Empty();
	}
}

void UGEP_SaveSubsystem::StartNewGame()
{
	PendingLoad = nullptr;
	SessionCollectedCoins.Empty();
}

bool UGEP_SaveSubsystem::RequestContinue()
{
	if (HasSave())
	{
		UGEP_SaveGame* Data = Cast<UGEP_SaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		
		if (!Data)
		{
			UE_LOG(LogTemp, Warning, TEXT("Error, save in slot %s is unreadable or corrupted"), SlotName);
			return false;
		}
		
		if (Data->SaveVersion == UGEP_SaveGame::CurrentVersion)
		{
			PendingLoad = Data;
			SessionCollectedCoins = Data->CollectedCoins;
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Error, save version (disk) %d does not match (newest) %d"), Data->SaveVersion, UGEP_SaveGame::CurrentVersion);
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Error, there are not saved files called %s"), SlotName);
	return false;
}

UGEP_SaveGame* UGEP_SaveSubsystem::ConsumePendingLoad()
{
	UGEP_SaveGame* Retval = PendingLoad;
	PendingLoad = nullptr;
	return Retval;
}

bool UGEP_SaveSubsystem::SaveAtCheckpoint(const FTransform& RespawnTransform, int32 MaxHP, int32 Coins) const
{
	UGEP_SaveGame* Data = Cast<UGEP_SaveGame>(UGameplayStatics::CreateSaveGameObject(UGEP_SaveGame::StaticClass())); 
	
	if (!Data)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impossible to create a SaveGameObject"));
		return false;
	}
	
	Data->RespawnTransform = RespawnTransform;
	Data->MaxHP = MaxHP;
	Data->Coins = Coins;
	Data->CollectedCoins = SessionCollectedCoins;
	
	return UGameplayStatics::SaveGameToSlot(Data, SlotName, 0);
}

void UGEP_SaveSubsystem::NotifyCoinCollected(FName CoinName)
{
	SessionCollectedCoins.Add(CoinName);
}

bool UGEP_SaveSubsystem::IsCoinCollected(FName CoinName) const
{
	return SessionCollectedCoins.Contains(CoinName);
}
