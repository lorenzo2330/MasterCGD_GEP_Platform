// Checkpoint.cpp
#include "Checkpoint.h"
#include "GEP_PlatformCharacter.h"
#include "GEP_PlatformGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	// Origine a metà altezza del box: il pavimento è a Z = -150
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->InitBoxExtent(FVector(100.f, 100.f, 150.f));
	Trigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	SetRootComponent(Trigger);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RespawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("RespawnPoint"));
	RespawnPoint->SetupAttachment(Trigger);
	RespawnPoint->SetRelativeLocation(FVector(0.f, 0.f, -150.f));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlap);
}

void ACheckpoint::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivated)
	{
		return;
	}

	AGEP_PlatformCharacter* Player = Cast<AGEP_PlatformCharacter>(OtherActor);
	AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>();
	if (!Player || !Player->IsAlive() || !GM)
	{
		return;
	}

	bActivated = true;

	// Il pivot di un Character è il centro della capsula: si sale di mezza altezza
	FVector Location = RespawnPoint->GetComponentLocation();
	Location.Z += Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GM->SetRespawnTransform(FTransform(RespawnPoint->GetComponentRotation(), Location));

	OnActivated();
}