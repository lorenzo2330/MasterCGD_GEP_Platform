// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealth::BeginPlay()
{
	Super::BeginPlay();
	
	HP = MaxHP;
}

void UHealth::ResetHealth()
{
	if (HP == MaxHP) { IncreaseMaxHP(); }
	HP = MaxHP;
	OnHealthChanged.Broadcast(HP, MaxHP);
	UE_LOG(LogTemp, Display, TEXT("Healthed -> Remains %d HP"), HP);
}

void UHealth::IncreaseMaxHP()
{
	MaxHP = FMath::Min(MaxHP + 1, LimitMaxHP);
	OnHealthChanged.Broadcast(HP, MaxHP);
	UE_LOG(LogTemp, Display, TEXT("HP recharged -> New max = %d"), MaxHP);
}

bool UHealth::DecreaseHP()
{
	HP = FMath::Max(HP - 1, 0);
	UE_LOG(LogTemp, Display, TEXT("Hurted -> Remains %d HP"), HP);
	OnHealthChanged.Broadcast(HP, MaxHP);
	if (HP <= 0) { OnDeath.Broadcast();  }
	return HP > 0;
}


