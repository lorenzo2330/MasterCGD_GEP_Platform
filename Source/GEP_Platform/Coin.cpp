#include "Coin.h"

#include "GEP_Platform.h"
#include "GEP_PlatformCharacter.h"
#include "GEP_PlayerState.h"
#include "GEP_SaveSubsystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(50.f);
	
	//Usa lo stesso profilo delle zone del nemico
	Trigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); 
	SetRootComponent(Trigger);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Rotation = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
	Rotation->RotationRate = FRotator(0.f, 180.f, 0.f);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlap);
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
	//Se la moneta è già stata raccolta, la distruggo
	const UGEP_SaveSubsystem* SaveSubsystem = UGEP_SaveSubsystem::Get(this);
	if (SaveSubsystem && SaveSubsystem->IsCoinCollected(GetFName())) { Destroy(); }
}

void ACoin::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCollected) { return; }
	

	AGEP_PlatformCharacter* Player = Cast<AGEP_PlatformCharacter>(OtherActor);
	if (!Player || !Player->IsAlive()) { return; }

	AGEP_PlayerState* PS = Player->GetPlayerState<AGEP_PlayerState>();
	if (!PS)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("Player has no AGEP_PlayerState: set Player State Class in BP_ThirdPersonGameMode."));
		return;
	}
	

	bCollected = true;
	
	//Notifico che ho raccolto una moneta
	if (UGEP_SaveSubsystem* Save = UGEP_SaveSubsystem::Get(this)) { Save->NotifyCoinCollected(GetFName()); }
	
	Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(false);
	PS->AddCoins(Value);

	OnCollected();
	if (CollectedLifeSpan > 0.f) { SetLifeSpan(CollectedLifeSpan); } else { Destroy(); }
	
}
