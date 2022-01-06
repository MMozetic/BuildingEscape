// Copyright Milos Mozetic 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, meta = (ClampMin="0.0", ClampMax="1.0"))
	float RotationOpenSpeed = 1.0;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RotationCloseSpeed = 1.0;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	AActor* ActorThatCanOpenDoor;
	float DoorLastOpened = 0.0;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.0;

private:
	void RotateDoorYaw(float DeltaTime, float &StartingYaw, float EndingYaw, float RotationSpeed);
	float CurrentYaw;
	float TargetYaw;
};
