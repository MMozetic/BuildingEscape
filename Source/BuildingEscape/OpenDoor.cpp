// Copyright Milos Mozetic 2022


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = CurrentYaw + OpenAngle;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no PressurePlate set."), *GetOwner()->GetName());
	}
	ActorThatCanOpenDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatCanOpenDoor))
	{
		// Open Door -> Rotate from Current Yaw to Target Yaw.
		RotateDoorYaw(DeltaTime, CurrentYaw, TargetYaw, RotationOpenSpeed);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		// Close Door -> Rotate from Current Yaw to Initial Yaw.
		if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorCloseDelay)
		{
			RotateDoorYaw(DeltaTime, CurrentYaw, TargetYaw - OpenAngle, RotationCloseSpeed);
		}
	}

}

void UOpenDoor::RotateDoorYaw(float DeltaTime, float &StartingYaw, float EndingYaw, float RotationSpeed)
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	StartingYaw = FMath::Lerp(StartingYaw, EndingYaw, RotationSpeed * DeltaTime);

	DoorRotation.Yaw = StartingYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}