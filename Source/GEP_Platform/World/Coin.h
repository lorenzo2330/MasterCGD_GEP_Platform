#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;

UCLASS()
class GEP_PLATFORM_API ACoin : public AActor
{
	GENERATED_BODY()

public:
	ACoin();

protected:
	virtual void BeginPlay() override;
	
	//Hook Blueprint per VFX e audio di raccolta
	UFUNCTION(BlueprintImplementableEvent, Category = "Coin")
	void OnCollected();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URotatingMovementComponent> Rotation;

	//Valore di una moneta (se si vogliono fare monete con valori diversi)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin", meta = (ClampMin = "1"))
	int32 Value = 1;

	//Tempo prima della distruzione, per lasciar finire VFX/audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin")
	float CollectedLifeSpan = 0.3f;

private:
	bool bCollected = false;
};