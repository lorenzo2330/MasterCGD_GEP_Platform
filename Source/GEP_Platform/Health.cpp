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
	
	StartingLocation = GetOwner()->GetActorLocation();
	StartingRotation = GetOwner()->GetActorRotation();
	
	UE_LOG(LogTemp, Display, TEXT("Start position -> %s"), *StartingLocation.ToString());
}

void UHealth::RefillHealth()
{
	if (HP == MaxHP) { IncreaseMaxHP(); }
	HP = MaxHP;
	UE_LOG(LogTemp, Display, TEXT("Healthed -> Remains %d HP"), HP);
}

void UHealth::IncreaseMaxHP()
{
	MaxHP = FMath::Min(MaxHP + 1, 5);
	UE_LOG(LogTemp, Display, TEXT("HP recharged -> New max = %d"), MaxHP);
}

bool UHealth::DecreaseHP()
{
	HP = FMath::Max(HP - 1, 0);
	UE_LOG(LogTemp, Display, TEXT("Hurted -> Remains %d HP"), HP);
	if (HP <= 0) { Respawn(); HP = MaxHP; }
	return HP > 0;
}

void UHealth::Respawn()
{
	UE_LOG(LogTemp, Display, TEXT("GAME OVER -> Respawn in %s"), *StartingRotation.ToString());
	GetOwner()->SetActorLocation(StartingLocation);
	GetOwner()->SetActorRotation(StartingRotation);
}

