#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "GEP_Boss.generated.h"

UCLASS(Abstract)
class GEP_PLATFORM_API AGEP_Boss : public AEnemyBase
{
	GENERATED_BODY()

public:
	virtual bool CanBeStomped() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SetVulnerable(bool bNewVulnerable);
	
	UFUNCTION(BlueprintPure, Category = "Boss")
	bool IsVulnerable() const { return bIsVulnerable; }
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	virtual void HandleDeath() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss")
	void OnVulnerabilityChanged(bool bNewVulnerable);
	
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Boss")
	bool bIsVulnerable = false;
	
	bool bDefeated = false;
};
