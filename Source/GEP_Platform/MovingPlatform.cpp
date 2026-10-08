
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
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		FVector NewStartLocation = StartLocation + MoveDirection * MaxDistance;
		StartLocation = NewStartLocation;
		PlatformVelocity *= -1;
	}
	
	SetActorLocation(CurrentLocation + PlatformVelocity * DeltaTime);
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("MovingPlatform::RotatePlatform"));
	AddActorLocalRotation(PlatformRotationVelocity * DeltaTime);
}

