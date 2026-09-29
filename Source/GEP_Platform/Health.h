// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GEP_PLATFORM_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();
	
	FVector StartingLocation;
	FRotator StartingRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHP = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP = 3;

	virtual void BeginPlay();
	
protected:
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void RefillHealth();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHP();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool DecreaseHP();
	
	void Respawn();
	
};
