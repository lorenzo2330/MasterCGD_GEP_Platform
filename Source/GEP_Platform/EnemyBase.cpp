// GEP_EnemyBase.cpp
#include "EnemyBase.h"

#include "AIController.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GEP_PlatformCharacter.h"
#include "Health.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Tick is enabled only during the death squash.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Needed to patrol also when spawned at runtime (the boss will be spawned too).
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
	
	MeshStartScale = GetMesh()->GetRelativeScale3D();
	StartPatrol();
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DeathElapsed += DeltaSeconds;
	const float Alpha = DeathLifeSpan > 0.f ? FMath::Clamp(DeathElapsed / DeathLifeSpan, 0.f, 1.f) : 1.f;
	GetMesh()->SetRelativeScale3D(MeshStartScale * FMath::Lerp(FVector::OneVector, DeathSquashScale, Alpha));
}

bool AEnemyBase::CanBeStomped() const
{
	return IsAlive();
}

bool AEnemyBase::OnStomped(AActor* Stomper)
{
	return Health->DecreaseHP();
}

void AEnemyBase::ResolveContact(AGEP_PlatformCharacter* Player, EContactZone Zone)
{
	if (!Player || !Player->IsAlive() || !IsAlive()) { return; }

	// Grace window after a stomp: avoids a second hit when both zones fire or several enemies are close.
	//Evita un secondo stomp se il player è ancora legato allo stomp precedente (finestra di tolleranza) 
	if (Player->IsInStompGrace()) { return; }

	if (IsStompValid(Player))
	{
		//Se il player è vulnerabile, subisce danno
		if (CanBeStomped()) { OnStomped(Player); }
		
		//A prescindere dall'invulnerabilità, il player rimbalza dopo lo stomp sul nemico
		Player->Bounce();
		return;
	}

	if (Zone == EContactZone::Hurt)	{ Player->GetHealth()->DecreaseHP(); }
}

void AEnemyBase::RecheckContact(AGEP_PlatformCharacter* Player)
{
	if (Player && HurtZone->IsOverlappingComponent(Player->GetCapsuleComponent()))
	{
		ResolveContact(Player, EContactZone::Hurt);
	}
}

bool AEnemyBase::IsAlive() const
{
	return Health && Health->IsAlive();
}

void AEnemyBase::OnStompZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGEP_PlatformCharacter* Player = Cast<AGEP_PlatformCharacter>(OtherActor);
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

void AEnemyBase::HandleDeath()
{
	//Disattiva le collisioni in modo che non si possano generare ulteriori eventi
	SetActorEnableCollision(false);
	GetCharacterMovement()->DisableMovement();
	StopPatrol();

	DeathElapsed = 0.f;
	SetActorTickEnabled(true);

	OnDefeated();
	SetLifeSpan(DeathLifeSpan);
}

void AEnemyBase::StartPatrol()
{
	if (PatrolPoints.Num() < 2) { return; }

	AAIController* AIC = Cast<AAIController>(GetController());
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: no AIController, patrol disabled."), *GetName());
		return;
	}

	//Mappa subito i punti di patorl per evitare problemi agli stessi durante il movimento
	const FTransform ActorTransform = GetActorTransform();
	for (const FVector& Local : PatrolPoints)
	{
		WorldPatrolPoints.Add(ActorTransform.TransformPosition(Local));
	}

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	AIC->ReceiveMoveCompleted.AddDynamic(this, &AEnemyBase::OnPatrolMoveCompleted);

	PatrolIndex = 0;
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::MoveToCurrentPatrolPoint, PatrolWaitTime, false);
}

void AEnemyBase::StopPatrol()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->ReceiveMoveCompleted.RemoveDynamic(this, &AEnemyBase::OnPatrolMoveCompleted);
		AIC->StopMovement();
	}
}

void AEnemyBase::MoveToCurrentPatrolPoint()
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (!IsAlive() || !AIC || !WorldPatrolPoints.IsValidIndex(PatrolIndex))
	{
		return;
	}
	AIC->MoveToLocation(WorldPatrolPoints[PatrolIndex], PatrolAcceptanceRadius);
}

void AEnemyBase::OnPatrolMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!IsAlive() || Result == EPathFollowingResult::Aborted) { return; }

	const int32 Last = WorldPatrolPoints.Num() - 1;

	//Se il patrol è stato completato, inverte la direzione, altrimenti torna all'ultimo punto di patrol
	if (Result == EPathFollowingResult::Success)
	{
		if (PatrolIndex + PatrolDirection > Last || PatrolIndex + PatrolDirection < 0)
		{
			PatrolDirection = -PatrolDirection;
		}
		PatrolIndex += PatrolDirection;
	}
	else
	{
		PatrolDirection = -PatrolDirection;
		PatrolIndex = FMath::Clamp(PatrolIndex + PatrolDirection, 0, Last);
	}

	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::MoveToCurrentPatrolPoint, PatrolWaitTime, false);
}

bool AEnemyBase::IsStompValid(const AGEP_PlatformCharacter* Player) const
{
	const bool bFalling = Player->GetVelocity().Z < 0.f;

	const float PlayerBaseZ = Player->GetActorLocation().Z - Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float EnemyTopZ = GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	return bFalling && PlayerBaseZ >= EnemyTopZ - StompHeightTolerance;
}
