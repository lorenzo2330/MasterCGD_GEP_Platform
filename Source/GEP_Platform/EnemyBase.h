// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stompable.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UHealth;
class UBoxComponent;
class AGEP_PlatformCharacter;

/** Which zone of the enemy generated the contact. */
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
	// Sets default values for this character's properties
	AEnemyBase();
	
	virtual bool CanBeStomped() const override;
	virtual bool OnStomped(AActor* Stomper) override;
	
	/** Single entry point for player contact. Public so the player can re-check overlaps when i-frames end. */
	void ResolveContact(AGEP_PlatformCharacter* Player, EContactZone Zone);
	
	/** Re-checks a contact that began while the player was invulnerable (called when i-frames end). */
	void RecheckContact(AGEP_PlatformCharacter* Player);
 
	bool IsAlive() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Geometric stomp check: player falling and capsule base above the enemy top (minus tolerance). */
	bool IsStompValid(const AGEP_PlatformCharacter* Player) const;
 
	UFUNCTION()
	void OnStompZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void OnHurtZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	/** Bound to Health->OnDeath. Disables collisions immediately, then schedules destruction. */
	UFUNCTION()
	void HandleDeath();
 
	/** Hook for death feedback (VFX, sound, scale-down) in the child Blueprint. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnDefeated();
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UHealth> Health;
 
	/** Top of the enemy: valid stomp area. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Contact")
	TObjectPtr<UBoxComponent> StompZone;
 
	/** Rest of the body: hurts the player. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Contact")
	TObjectPtr<UBoxComponent> HurtZone;
 
	/** How far (cm) the player capsule base may be below the enemy top and still count as a stomp. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Contact", meta = (ClampMin = "0"))
	float StompHeightTolerance = 30.f;
 
	/** Seconds before the actor is destroyed after death. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (ClampMin = "0"))
	float DeathLifeSpan = 0.5f;
};
