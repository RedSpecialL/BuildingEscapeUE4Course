// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "WeakObjectPtr.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find physics handle component of the %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	// TODO: Find way to add c++17
	// FVector [Start, End] = GetReachLineEnds();
	
	FTwoVectors ReachLine{ std::move(GetReachLineEnds()) };
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ReachLine.v1,
		ReachLine.v2,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return Hit;
}

FTwoVectors UGrabber::GetReachLineEnds() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRatation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRatation);

	FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRatation.Vector() * ReachCm;
	return FTwoVectors(PlayerViewPointLocation, LineEnd);
}

void UGrabber::Grab()
{
	const FHitResult FoundBody{ GetFirstPhysicsBodyInReach() };
	UPrimitiveComponent* ComponentToGrab = FoundBody.GetComponent();
	const AActor* Actor = FoundBody.GetActor();

	/// If we hit something then attach a physics handle
	if (Actor != nullptr)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bones, because we grab static mesh
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::GrabReleased()
{
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached Input Component (only appears at run time).
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputComponent != nullptr)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find imput component of the %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(
	float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent != nullptr)
	{
		/// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnds().v2);
	}
}
