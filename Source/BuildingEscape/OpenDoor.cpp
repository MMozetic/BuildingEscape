// Copyright Milos Mozetic 2022


#include "OpenDoor.h"
#include "Components/AudioComponent.h"

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

	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > RequiredMassToOpenDoors)
	{
		// Open Door -> Rotate from Current Yaw to Target Yaw.
		RotateDoorYaw(DeltaTime, CurrentYaw, TargetYaw, RotationOpenSpeed);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		if (!IsDoorSoundPlaying && AudioComponent) {
			IsDoorSoundPlaying = true;
			AudioComponent->Play();
		}
	}
	else
	{
		// Close Door -> Rotate from Current Yaw to Initial Yaw.
		if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorCloseDelay)
		{
			RotateDoorYaw(DeltaTime, CurrentYaw, TargetYaw - OpenAngle, RotationCloseSpeed);
		}

		if (IsDoorSoundPlaying)
		{
			IsDoorSoundPlaying = false;
			AudioComponent->Play();
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

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate)
	{
		return TotalMass;
	}

	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}


void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't contain audio component."), *GetOwner()->GetName());
	}
}