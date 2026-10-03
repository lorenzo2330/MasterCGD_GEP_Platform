// GEP_EnemyBase.cpp
#include "EnemyBase.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GEP_PlatformCharacter.h"
#include "Health.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// The enemy capsule must not block the player: contact is handled only by the two zones (D-003).
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	const float HalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	// Default sizes assume the default ACharacter capsule; tune them in the child Blueprint.
	StompZone = CreateDefaultSubobject<UBoxComponent>(TEXT("StompZone"));
	StompZone->SetupAttachment(GetCapsuleComponent());
	StompZone->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	StompZone->SetBoxExtent(FVector(40.f, 40.f, 20.f));
	StompZone->SetRelativeLocation(FVector(0.f, 0.f, HalfHeight - 10.f));

	HurtZone = CreateDefaultSubobject<UBoxComponent>(TEXT("HurtZone"));
	HurtZone->SetupAttachment(GetCapsuleComponent());
	HurtZone->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	HurtZone->SetBoxExtent(FVector(40.f, 40.f, 70.f));
	HurtZone->SetRelativeLocation(FVector(0.f, 0.f, -10.f));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	StompZone->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnStompZoneBeginOverlap);
	HurtZone->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnHurtZoneBeginOverlap);
	Health->OnDeath.AddDynamic(this, &AEnemyBase::HandleDeath);
}

bool AEnemyBase::IsAlive() const
{
	return Health && Health->IsAlive();
}

bool AEnemyBase::CanBeStomped() const
{
	// Override in the boss to restrict stomps to its vulnerability windows.
	return IsAlive();
}

bool AEnemyBase::OnStomped(AActor* Stomper)
{
	return Health->DecreaseHP();
}

void AEnemyBase::OnStompZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGEP_PlatformCharacter* Player = Cast<AGEP_PlatformCharacter>(OtherActor);
	// Only the capsule counts (the skeletal mesh could also generate overlaps).
	if (Player && OtherComp == Player->GetCapsuleComponent())
	{
		ResolveContact(Player, EContactZone::Stomp);
	}
}

void AEnemyBase::OnHurtZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGEP_PlatformCharacter* Player = Cast<AGEP_PlatformCharacter>(OtherActor);
	if (Player && OtherComp == Player->GetCapsuleComponent())
	{
		ResolveContact(Player, EContactZone::Hurt);
	}
}

bool AEnemyBase::IsStompValid(const AGEP_PlatformCharacter* Player) const
{
	const bool bFalling = Player->GetVelocity().Z < 0.f;

	const float PlayerBaseZ = Player->GetActorLocation().Z - Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float EnemyTopZ = GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	return bFalling && PlayerBaseZ >= EnemyTopZ - StompHeightTolerance;
}

void AEnemyBase::ResolveContact(AGEP_PlatformCharacter* Player, EContactZone Zone)
{
	// Dead player or dead enemy: contact ignored.
	if (!Player || !Player->IsAlive() || !IsAlive())
	{
		return;
	}

	// Grace window after a stomp: avoids a second hit when both zones fire or several enemies are close.
	if (Player->IsInStompGrace())
	{
		return;
	}

	if (IsStompValid(Player))
	{
		// Valid stomp: if the enemy is not vulnerable (e.g. boss) the player still bounces, nobody takes damage.
		if (CanBeStomped())
		{
			OnStomped(Player);
		}
		Player->Bounce();
		return;
	}

	// Invalid stomp: damage only from the HurtZone; touching the StompZone does nothing.
	if (Zone == EContactZone::Hurt)
	{
		Player->GetHealth()->DecreaseHP(); // ignored internally during i-frames
	}
}

void AEnemyBase::RecheckContact(AGEP_PlatformCharacter* Player)
{
	if (Player && HurtZone->IsOverlappingComponent(Player->GetCapsuleComponent()))
	{
		ResolveContact(Player, EContactZone::Hurt);
	}
}

void AEnemyBase::HandleDeath()
{
	// Disable collisions at once so the same frame cannot produce further contacts (D-003).
	SetActorEnableCollision(false);
	GetCharacterMovement()->DisableMovement();

	OnDefeated();
	SetLifeSpan(DeathLifeSpan);
}