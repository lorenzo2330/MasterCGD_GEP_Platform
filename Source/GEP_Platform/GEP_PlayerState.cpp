#include "GEP_PlayerState.h"
#include "Health.h"
#include "GameFramework/Pawn.h"

void AGEP_PlayerState::AddCoins(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	const int32 OldBonuses = Coins / CoinsPerBonus;
	Coins += Amount;
	const int32 NewBonuses = Coins / CoinsPerBonus;

	OnCoinsChanged.Broadcast(Coins);

	if (NewBonuses > OldBonuses)
	{
		if (APawn* Pawn = GetPawn())
		{
			if (UHealth* Health = Pawn->FindComponentByClass<UHealth>())
			{
				for (int32 i = OldBonuses; i < NewBonuses; ++i)
				{
					Health->RefillHealth();
				}
			}
		}
		OnBonusGranted.Broadcast();
	}
}

void AGEP_PlayerState::SetCoins(int32 NewTotal)
{
	Coins = FMath::Max(0, NewTotal);
	OnCoinsChanged.Broadcast(Coins);
}

int32 AGEP_PlayerState::GetCoinsTowardBonus() const
{
	return Coins % CoinsPerBonus;
}