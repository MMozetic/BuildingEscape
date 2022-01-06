// Copyright Milos Mozetic 2022


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicshandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grab pressed"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	
	if (HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacted with actor: %s"), *HitResult.GetActor()->GetName());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Grab released"));
}

void UGrabber::FindPhysicshandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Display, TEXT("%s contains PhysicsHandle."), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't contain PhysicsHandle Component. Please add it."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't contain InputComponent Component. Please add it."), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector RayCastEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		PlayerViewPointLocation,
		RayCastEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(TEXT(""), false, GetOwner())
	);

	return HitResult;
}
