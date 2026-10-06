// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

class UGEP_SaveSubsystem;
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

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(Category = "Health")
	void ResetHealth();
	
	UFUNCTION(Category = "Health")
	void RefillHealth();
	
	UFUNCTION(Category = "Health")
	void IncreaseMaxHP();
	
	UFUNCTION(Category = "Health")
	bool DecreaseHP();
	
	UFUNCTION(Category = "Health")
	bool IsAlive() const { return HP > 0; }
	
	int32 GetCurrentHP() const { return HP; }
	int32 GetMaxHP() const { return MaxHP; }

	void RestoreFromSave(int32 SavedMaxHP);
	
	FOnDeath OnDeath;
	FOnInvulnerabilityChanged OnInvulnerabilityChanged;
	FOnHealthChanged OnHealthChanged;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category = "Health|HP")
	int32 LimitMaxHP = 5;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 MaxHP = 3;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 HP;
	
	bool bIsInvulnerable = false;
	FTimerHandle InvulnerabilityTimer;

	void StartInvulnerability();
	void EndInvulnerability();
};
