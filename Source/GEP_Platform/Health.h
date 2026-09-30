// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, CurrentHP, int32, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GEP_PLATFORM_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();
	
	UPROPERTY(VisibleAnywhere, Category = "Health|HP")
	int32 LimitMaxHP = 5;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 MaxHP = 3;
	
	UPROPERTY(EditAnywhere, Category = "Health|HP")
	int32 HP;
	
	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDeath OnDeath;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ResetHealth();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHP();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool DecreaseHP();

protected:
	
	
	
};
