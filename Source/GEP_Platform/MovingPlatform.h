#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class GEP_PLATFORM_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatform();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	

	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Platform")
	float MaxDistance = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Platform")
	FVector PlatformVelocity = FVector(0.0f, 0.0f, 100.0f);

	UPROPERTY(EditAnywhere, Category = "Platform")
	FRotator PlatformRotationVelocity = FRotator(0.0f, 0.0f, 0.0f);

	void MovePlatform(float DeltaTime);

	void RotatePlatform(float DeltaTime);
	
};
