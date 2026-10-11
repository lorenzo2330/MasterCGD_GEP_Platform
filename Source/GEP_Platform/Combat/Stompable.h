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

class GEP_PLATFORM_API IStompable
{
	GENERATED_BODY()

public:
	//Per permettere ad un nemico di essere vulnerabile solo temporaneamente (es durante una bossfight)
	virtual bool CanBeStomped() const = 0;
 
	//Risponde all'azione di stomp (true se viene accettato (inflitto danno), false altrimenti)
	virtual bool OnStomped(AActor* Stomper) = 0;
};
