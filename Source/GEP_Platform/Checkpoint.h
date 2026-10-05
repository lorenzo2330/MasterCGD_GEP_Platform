// Checkpoint.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;

/** Checkpoint: al primo passaggio del player aggiorna il punto di respawn nel GameMode. */
UCLASS()
class GEP_PLATFORM_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckpoint();

protected:
	//Hook Blueprint per feedback (luce, bandiera, suono)
	UFUNCTION(BlueprintImplementableEvent, Category = "Checkpoint")
	void OnActivated();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	//Punto di respawn a livello del pavimento, la freccia mostra la direzione
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UArrowComponent> RespawnPoint;

private:
	bool bActivated = false;
};