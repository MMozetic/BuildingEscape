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
	TargetYaw = CurrentYaw + OffsetYaw;

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
		OpenDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, LerpAlpha * DeltaTime);

	if (CurrentYaw != OpenDoor.Yaw)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Rotation: %s"), *GetOwner()->GetActorRotation().ToString());
	}

	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);
}

