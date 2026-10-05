#include "GEP_UserWidget.h"
#include "Health.h"
#include "GEP_PlayerState.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void UGEP_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		// Se il pawn viene posseduto dopo la creazione del widget
		PC->OnPossessedPawnChanged.AddDynamic(this, &UGEP_UserWidget::HandlePawnChanged);
	}
	TryBind();
}

void UGEP_UserWidget::NativeDestruct()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->OnPossessedPawnChanged.RemoveDynamic(this, &UGEP_UserWidget::HandlePawnChanged);
	}
	Unbind();
	Super::NativeDestruct();
}

void UGEP_UserWidget::TryBind()
{
	Unbind();

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	if (APawn* Pawn = PC->GetPawn())
	{
		if (UHealth* Health = Pawn->FindComponentByClass<UHealth>())
		{
			BoundHealth = Health;
			Health->OnHealthChanged.AddDynamic(this, &UGEP_UserWidget::HandleHealthChanged);
			HandleHealthChanged(Health->GetCurrentHP(), Health->GetMaxHP());
		}
	}

	if (AGEP_PlayerState* PS = PC->GetPlayerState<AGEP_PlayerState>())
	{
		BoundPlayerState = PS;
		PS->OnCoinsChanged.AddDynamic(this, &UGEP_UserWidget::HandleCoinsChanged);
		UpdateCoinsText(PS);
	}
}

void UGEP_UserWidget::Unbind()
{
	if (BoundHealth.IsValid())
	{
		BoundHealth->OnHealthChanged.RemoveDynamic(this, &UGEP_UserWidget::HandleHealthChanged);
	}
	if (BoundPlayerState.IsValid())
	{
		BoundPlayerState->OnCoinsChanged.RemoveDynamic(this, &UGEP_UserWidget::HandleCoinsChanged);
	}
	BoundHealth.Reset();
	BoundPlayerState.Reset();
}

void UGEP_UserWidget::HandleHealthChanged(int32 CurrentHP, int32 MaxHP) const
{
	TextHealth->SetText(FText::FromString(FString::Printf(TEXT("HP: %d / %d"), CurrentHP, MaxHP)));
}

void UGEP_UserWidget::HandleCoinsChanged(int32 TotalCoins) const
{
	UpdateCoinsText(BoundPlayerState.Get());
}

void UGEP_UserWidget::HandlePawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	TryBind();
}

void UGEP_UserWidget::UpdateCoinsText(const AGEP_PlayerState* PS) const
{
	if (!PS)
	{
		return;
	}
	TextCoins->SetText(FText::FromString(FString::Printf(TEXT("Monete: %d (%d/%d)"),
		PS->GetCoins(), PS->GetCoinsTowardBonus(), PS->CoinsPerBonus)));
}
