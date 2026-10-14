#include "GEP_UserWidget.h"
#include "Combat/Health.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GEP_PlayerState.h"
#include "Components/TextBlock.h"
#include "Enemies/GEP_Boss.h"
#include "Game/GEP_PlatformGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void UGEP_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		//Se il pawn viene posseduto dopo la creazione del widget
		PC->OnPossessedPawnChanged.AddDynamic(this, &UGEP_UserWidget::HandlePawnChanged);
	}
	
	TryBind();
	TryBindBoss(Cast<AGEP_Boss>(UGameplayStatics::GetActorOfClass(GetWorld(), AGEP_Boss::StaticClass())));
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
	
	if (AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>())
	{
		BoundGameMode = GM;
		GM->OnEnemyCountChanged.AddDynamic(this, &UGEP_UserWidget::HandleNumEnemyChanged);
		UpdateNumEnemyText(GM);
	}
}

void UGEP_UserWidget::TryBindBoss(AGEP_Boss* Boss)
{
	if (!Boss) { return; }
	
	UHealth* BossHealth = Boss->GetComponentByClass<UHealth>();
	
	if (!BossHealth) { return; }
	
	BoundBossHealth = BossHealth;
	BossHealth->OnHealthChanged.AddDynamic(this, &UGEP_UserWidget::HandleBossHealthChanged);
	HandleBossHealthChanged(BossHealth->GetCurrentHP(), BossHealth->GetMaxHP());
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
	TextHealth->SetText(FText::FromString(FString::Printf(TEXT("HP :  %d / %d"), CurrentHP, MaxHP)));
}

void UGEP_UserWidget::HandleBossHealthChanged(int32 CurrentHP, int32 MaxHP) const
{
	TextBossHealth->SetText(FText::FromString(FString::Printf(TEXT("Boss HP :  %d / %d"), CurrentHP, MaxHP)));
}

void UGEP_UserWidget::HandleCoinsChanged(int32 TotalCoins) const
{
	UpdateCoinsText(BoundPlayerState.Get());
}

void UGEP_UserWidget::HandleNumEnemyChanged(int32 CurrentNumEnemy, int32 TotalNumEnemy) const
{
	UpdateNumEnemyText(BoundGameMode.Get());
}

void UGEP_UserWidget::HandlePawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	TryBind();
}

void UGEP_UserWidget::UpdateCoinsText(const AGEP_PlayerState* PS) const
{
	if (!PS) { return; }
	TextCoins->SetText(FText::FromString(FString::Printf(TEXT("Coins :  %d (%d/%d)"),
		PS->GetCoins(), PS->GetCoinsTowardBonus(), PS->CoinsPerBonus)));
}

void UGEP_UserWidget::UpdateNumEnemyText(const AGEP_PlatformGameMode* GM) const
{
	if (!GM) { return; }
	TextNumEnemies->SetText(FText::FromString(FString::Printf(TEXT("Enemies :  %d / %d"), GM->GetAliveEnemies(), GM->GetTotalEnemies())));
}
