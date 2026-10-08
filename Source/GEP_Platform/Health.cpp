// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

#include "GEP_Platform.h"

UHealth::UHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UHealth::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(InvulnerabilityTimer);
	}
	Super::EndPlay(EndPlayReason);
}

void UHealth::ResetHealth()
{
	HP = MaxHP;
	OnHealthChanged.Broadcast(HP, MaxHP);
	StartInvulnerability();
	UE_LOG(LogGEP_Platform, Verbose, TEXT("Reset -> %d HP"), HP);
}

void UHealth::RefillHealth()
{
	if (HP == MaxHP) { IncreaseMaxHP(); }
	HP = MaxHP;
	OnHealthChanged.Broadcast(HP, MaxHP);
	UE_LOG(LogGEP_Platform, Verbose, TEXT("Healthed -> %d HP"), HP);
}

void UHealth::IncreaseMaxHP()
{
	MaxHP = FMath::Min(MaxHP + 1, LimitMaxHP);
	OnHealthChanged.Broadcast(HP, MaxHP);
	UE_LOG(LogGEP_Platform, Verbose, TEXT("HP recharged -> New max = %d"), MaxHP);
}

bool UHealth::DecreaseHP()
{
	if (bIsInvulnerable || HP <= 0) { return false; }
	
	HP = FMath::Max(HP - 1, 0);
	UE_LOG(LogGEP_Platform, Verbose, TEXT("Hurted -> Remains %d HP"), HP);
	OnHealthChanged.Broadcast(HP, MaxHP);
	if (HP <= 0) { OnDeath.Broadcast();  }
	else { StartInvulnerability(); }
	return true;
}

void UHealth::RestoreFromSave(int32 SavedMaxHP)
{
	//Se SavedMaxHP è < di 1, restituisce 1 | Se SavedMaxHP > LimitMaxHP, restituisce LimitMaxHP | Altrimenti restituisce SavedMaxHP
	MaxHP = FMath::Clamp(SavedMaxHP, 1, LimitMaxHP);
	HP = MaxHP;
	OnHealthChanged.Broadcast(HP, MaxHP);
}

void UHealth::InitializeComponent()
{
	Super::InitializeComponent();
	HP = MaxHP;
}

void UHealth::StartInvulnerability()
{
	if (InvulnerabilityDuration <= 0.f) { return; }

	bIsInvulnerable = true;
	OnInvulnerabilityChanged.Broadcast(true);
	GetWorld()->GetTimerManager().SetTimer(InvulnerabilityTimer, this, &UHealth::EndInvulnerability, InvulnerabilityDuration, false);
}

void UHealth::EndInvulnerability()
{
	bIsInvulnerable = false;
	OnInvulnerabilityChanged.Broadcast(false);
}
