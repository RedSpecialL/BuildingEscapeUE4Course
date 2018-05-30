// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "WeakObjectPtr.h"


#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRatation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRatation);

	//  Log out to test.
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
	//	*PlayerViewPointLocation.ToString(), *PlayerViewPointRatation.ToString())

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRatation.Vector() * ReachCm;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor::Red,
		false,
		0,
		0,
		10
	);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	
	/// Line trace (Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	AActor* HitActor = Hit.GetActor();
	if(HitActor != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("%s was hitted."), *(HitActor->GetName()))
}
