
#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay(); 
	StartLocation = GetActorLocation();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MovePlatform(DeltaTime);
	
	if (!PlatformRotationVelocity.IsZero())
	{
		RotatePlatform(DeltaTime);
	}
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	
	if (FVector::Dist(StartLocation, CurrentLocation) >= MaxDistance)
	{
		StartLocation += PlatformVelocity.GetSafeNormal() * MaxDistance;
		PlatformVelocity *= -1;
	}
	
	SetActorLocation(CurrentLocation + PlatformVelocity * DeltaTime);
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(PlatformRotationVelocity * DeltaTime);
}

