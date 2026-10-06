// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEP_PlatformCharacter.h"

#include "EnemyBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Health.h"
#include "GEP_Platform.h"
#include "GEP_PlatformGameMode.h"
#include "GEP_PlayerState.h"
#include "GEP_SaveGame.h"
#include "GEP_SaveSubsystem.h"

class AGEP_PlatformGameMode;
class AEnemyBase;

AGEP_PlatformCharacter::AGEP_PlatformCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));
}

void AGEP_PlatformCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>())
	{
		GM->SetRespawnTransform(FTransform(GetActorRotation(), GetActorLocation()));

		UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this);
		if (UGEP_SaveGame* Data = Save ? Save->ConsumePendingLoad() : nullptr)
		{
			GM->SetRespawnTransform(Data->RespawnTransform);
			Health->RestoreFromSave(Data->MaxHP);

			if (AGEP_PlayerState* PS = GetPlayerState<AGEP_PlayerState>())
			{
				PS->SetCoins(Data->Coins);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerState not ready: coins not restored"));
			}
			RespawnAtCheckpoint();
		}
	}

	Health->OnDeath.AddDynamic(this, &AGEP_PlatformCharacter::HandleDeath);
	Health->OnInvulnerabilityChanged.AddDynamic(this, &AGEP_PlatformCharacter::HandleInvulnerabilityChanged);
}

void AGEP_PlatformCharacter::HandleDeath()
{
	if (AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>())
	{
		GM->HandlePlayerDeath();
	}
}

void AGEP_PlatformCharacter::RespawnAtCheckpoint()
{
	const AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	if (!GM)
	{
		return;
	}

	const FTransform& Target = GM->GetRespawnTransform();
	TeleportTo(Target.GetLocation(), Target.Rotator());
	GetCharacterMovement()->StopMovementImmediately();
	if (AController* C = GetController())
	{
		C->SetControlRotation(Target.Rotator());
	}
}

void AGEP_PlatformCharacter::ToggleMeshVisibility()
{
	GetMesh()->SetVisibility(!GetMesh()->IsVisible());
}

void AGEP_PlatformCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGEP_PlatformCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGEP_PlatformCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGEP_PlatformCharacter::Look);
	}
	else
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGEP_PlatformCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AGEP_PlatformCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGEP_PlatformCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AGEP_PlatformCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AGEP_PlatformCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AGEP_PlatformCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AGEP_PlatformCharacter::Bounce()
{
	// Keep horizontal velocity, override only Z
	LaunchCharacter(FVector(0.f, 0.f, BounceVelocity), /*bXYOverride*/ false, /*bZOverride*/ true);
	LastStompTime = GetWorld()->GetTimeSeconds();
}

bool AGEP_PlatformCharacter::IsInStompGrace() const
{
	return GetWorld()->GetTimeSeconds() - LastStompTime < StompGraceTime;
}

void AGEP_PlatformCharacter::HandleInvulnerabilityChanged(bool bIsInvulnerable)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (bIsInvulnerable)
	{
		TimerManager.SetTimer(BlinkTimer, this, &AGEP_PlatformCharacter::ToggleMeshVisibility, BlinkInterval, true);
	}
	else
	{
		TimerManager.ClearTimer(BlinkTimer);
		GetMesh()->SetVisibility(true);
		RecheckEnemyContacts(); // keep last: it may start new i-frames
	}
}

void AGEP_PlatformCharacter::RecheckEnemyContacts()
{
	TArray<AActor*> Overlapping;
	GetCapsuleComponent()->GetOverlappingActors(Overlapping, AEnemyBase::StaticClass());
	for (AActor* Actor : Overlapping)
	{
		if (AEnemyBase* Enemy = Cast<AEnemyBase>(Actor))
		{
			Enemy->RecheckContact(this);
		}
	}
}

void AGEP_PlatformCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	// Caduta sotto il Kill Z del livello
	
	if (!IsAlive()) { return; }

	GetHealth()->DecreaseHP();
	if (IsAlive()) { RespawnAtCheckpoint(); }
}