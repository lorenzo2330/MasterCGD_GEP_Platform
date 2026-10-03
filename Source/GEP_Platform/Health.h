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
	
	UPROPERTY(VisibleAnywhere, Category = "Health|HP")
	int32 LimitMaxHP = 5;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 MaxHP = 3;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 HP;
	
	UPROPERTY(EditAnywhere, Category = "Health|Invulnerability", meta = (ClampMin = "0.0"))
	float InvulnerabilityDuration = 1.5f;
	
	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnInvulnerabilityChanged OnInvulnerabilityChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDeath OnDeath;

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ResetHealth();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void RefillHealth();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHP();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool DecreaseHP();
	
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsInvulnerable() const { return bIsInvulnerable; }

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAlive() const { return HP > 0; }

private:
	bool bIsInvulnerable = false;
	FTimerHandle InvulnerabilityTimer;

	void StartInvulnerability();
	void EndInvulnerability();
	
	
};
