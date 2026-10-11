#include "GEP_Boss.h"

#include "GEP_Platform.h"
#include "GEP_PlatformGameMode.h"

bool AGEP_Boss::CanBeStomped() const
{
	return IsAlive() && bIsVulnerable;
}

void AGEP_Boss::SetVulnerable(bool bNewVulnerable)
{
	if (bNewVulnerable == bIsVulnerable) { return; }
	bIsVulnerable = bNewVulnerable;
	OnVulnerabilityChanged(bIsVulnerable);
}

void AGEP_Boss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!bDefeated || EndPlayReason != EEndPlayReason::Destroyed) { return; }
	
	UWorld* World = GetWorld();
	if (!World) { return; }
	
	AGEP_PlatformGameMode* GM = World->GetAuthGameMode<AGEP_PlatformGameMode>();
	if (!GM)
	{
		UE_LOG(LogGEP_Platform, Error, TEXT("Boss defeated but the GameMode is not AGEP_PlatformGameMode: victory not triggered. Check the World Settings GameMode Override."));
		return;
	}

	GM->TriggerVictory();
}

void AGEP_Boss::HandleDeath()
{
	Super::HandleDeath();
	SetVulnerable(false);
	bDefeated = true;
}
