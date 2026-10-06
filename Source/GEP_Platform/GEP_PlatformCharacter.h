// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Health.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GEP_PlatformCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

class UHealth;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AGEP_PlatformCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	//-----------------------------------------------HEALTH--------------------------------
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealth> Health;
	
	//-----------------------------------------------HEALTH--------------------------------
	//-----------------------------------------------STOMP---------------------------------	
	
	/** Bounce after a valid stomp; also starts the stomp grace window. */
	UFUNCTION(BlueprintCallable, Category = "Stomp")
	void Bounce();

	/** True shortly after a stomp: contacts with enemies are ignored. */
	bool IsInStompGrace() const;

	// Skip these two if they already exist
	UHealth* GetHealth() const { return Health; }
	bool IsAlive() const { return Health && Health->IsAlive(); }
	
	//-----------------------------------------------STOMP---------------------------------	
	//-----------------------------------------------GAMEFLOW------------------------------
	
	void RespawnAtCheckpoint();
	
	//-----------------------------------------------GAMEFLOW------------------------------
	
	
	
	
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	/** Constructor */
	AGEP_PlatformCharacter();	

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:

	virtual void BeginPlay() override;

	//-----------------------------------------------HEALTH--------------------------------
	
	/** Called when Health reaches 0 */
	UFUNCTION()
	void HandleDeath();
	
	/** Blink interval (seconds) while invulnerable */
	UPROPERTY(EditAnywhere, Category = "Health")
	float BlinkInterval = 0.1f;

	FTimerHandle BlinkTimer;

	/** Start/stop the blink feedback */
	UFUNCTION()
	void HandleInvulnerabilityChanged(bool bIsInvulnerable);

	void ToggleMeshVisibility();
	
	//-----------------------------------------------HEALTH--------------------------------
	//-----------------------------------------------STOMP---------------------------------	
	
	/** Upward speed (cm/s) of the bounce. The template jump is 500. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stomp", meta = (ClampMin = "0"))
	float BounceVelocity = 450.f;

	/** Seconds of protection after a stomp (D-003). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stomp", meta = (ClampMin = "0"))
	float StompGraceTime = 0.2f;
	
	float LastStompTime = -1000.f;
	
	/** Re-resolves contacts with enemies still overlapping when i-frames end. */
	void RecheckEnemyContacts();
	
	//-----------------------------------------------STOMP---------------------------------	
	//-----------------------------------------------GAMEFLOW------------------------------
	
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	
	//-----------------------------------------------GAMEFLOW------------------------------
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
};

