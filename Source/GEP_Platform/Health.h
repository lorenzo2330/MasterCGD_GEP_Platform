// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, CurrentHP, int32, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvulnerabilityChanged, bool, bIsInvulnerable);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GEP_PLATFORM_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealth();
	
	UPROPERTY(EditAnywhere, Category = "Health|Invulnerability", meta = (ClampMin = "0.0"))
	float InvulnerabilityDuration = 1.5f;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void ResetHealth();
	
	void RefillHealth();
	
	void IncreaseMaxHP();
	
	bool DecreaseHP();
	
	bool IsAlive() const { return HP > 0; }
	
	int32 GetCurrentHP() const { return HP; }
	int32 GetMaxHP() const { return MaxHP; }

	void RestoreFromSave(int32 SavedMaxHP);
	
	FOnDeath OnDeath;
	FOnInvulnerabilityChanged OnInvulnerabilityChanged;
	FOnHealthChanged OnHealthChanged;
	
	virtual void InitializeComponent() override;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category = "Health|HP")
	int32 LimitMaxHP = 5;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 MaxHP = 3;
	
	UPROPERTY(VisibleAnywhere, Category = "Health|HP")
	int32 HP = 0;
	
	bool bIsInvulnerable = false;
	FTimerHandle InvulnerabilityTimer;

	void StartInvulnerability();
	void EndInvulnerability();
};
