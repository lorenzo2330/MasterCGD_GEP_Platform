// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FVector StartLocation;

	UPROPERTY(VisibleAnywhere)
	float DistanceMoved = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 250.0f;

	UPROPERTY(EditAnywhere)
	FVector PlatformVelocity = FVector(0.0f, 0.0f, 100.0f);

	UPROPERTY(EditAnywhere)
	FRotator PlatformRotationVelocity = FRotator(0.0f, 0.0f, 0.0f);

	void MovePlatform(float DeltaTime);

	void RotatePlatform(float DeltaTime);
};
