// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Stompable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UStompable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GEP_PLATFORM_API IStompable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** True if a stomp can hurt this actor right now (e.g. false for a boss outside its vulnerability window). */
	virtual bool CanBeStomped() const = 0;
 
	/** Applies the stomp. Returns true if it was accepted (damage applied), false if ignored. */
	virtual bool OnStomped(AActor* Stomper) = 0;
};
