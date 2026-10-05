// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "CoreMinimal.h"
#include "Stompable.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


class UHealth;
class UBoxComponent;
class AGEP_PlatformCharacter;

UENUM()
enum class EContactZone : uint8
{
	Stomp,
	Hurt
};

UCLASS(Abstract)
class GEP_PLATFORM_API AEnemyBase : public ACharacter,  public IStompable
{
	GENERATED_BODY()

public:
	AEnemyBase();
	
	virtual void BeginPlay() override;
	
	//Usato solo durante l "schiacciamento" dovuto alla morte dell'enemy
	virtual void Tick(float DeltaSeconds) override;
	
	virtual bool CanBeStomped() const override;
	virtual bool OnStomped(AActor* Stomper) override;
	
	//Gestisce il contatto col player
	void ResolveContact(AGEP_PlatformCharacter* Player, EContactZone Zone);
	
	//Chiamata al termine degli i-frame
	void RecheckContact(AGEP_PlatformCharacter* Player);
 
	bool IsAlive() const;
	

protected:
 
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	TObjectPtr<UHealth> Health;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UBoxComponent> StompZone;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UBoxComponent> HurtZone;
 
	//Distanza max ("di compenetrazione") tra il player e il top dell'enemy 
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float StompHeightTolerance = 30.f;
 
	//Secondi dopo i quali il player viene distrutto
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float DeathLifeSpan = 0.5f;
	
	//Punti che definiscono il percorso di patrol
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (MakeEditWidget = "true"))
	TArray<FVector> PatrolPoints;

	//Velocità di patrol
	UPROPERTY(EditAnywhere, Category = "Enemy|Patrol", meta = (ClampMin = "0"))
	float PatrolSpeed = 150.f;

	//Tempo di attesa tra un segmento di patrol ed il successivo
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0.1"))
	float PatrolWaitTime = 1.f;

	//Cm di tolleranza dal patrol point per considerarlo raggiunto
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "5"))
	float PatrolAcceptanceRadius = 30.f;
	
	//Scale per fare lo squash del nemico quando viene colpito
	UPROPERTY(EditAnywhere, Category = "Enemy")
	FVector DeathSquashScale = FVector(1.3f, 1.3f, 0.1f);
	
	UFUNCTION()
	void OnStompZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void OnHurtZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

 
	UFUNCTION()
	void HandleDeath();
 
	//Chiamabile in BP per gestire i feedback di morte del player (VFX, sound, scale-down)
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnDefeated();

	UFUNCTION()
	void OnPatrolMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	void StartPatrol();
	void StopPatrol();
	void MoveToCurrentPatrolPoint();
	
	//Invocato in fase di collisione, controlla se il player sta cadendo e si trova all'altezza definita dalla zona di stomp
	bool IsStompValid(const AGEP_PlatformCharacter* Player) const;
	
private:
	TArray<FVector> WorldPatrolPoints;
	int32 PatrolIndex = 0;
	int32 PatrolDirection = 1;
	FTimerHandle PatrolTimer;

	FVector MeshStartScale = FVector::OneVector;
	float DeathElapsed = 0.f;
};
