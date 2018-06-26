// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/TriggerVolume.h"
#include "Components/PrimitiveComponent.h"
#include "Algo/Accumulate.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Cannot find PressurePlate of the %s"), *GetOwner()->GetName());
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	if (PressurePlate == nullptr)
	{
		return 0.0f;
	}

	TArray<AActor*> ActorsOnPlate;
	PressurePlate->GetOverlappingActors(OUT ActorsOnPlate);
	float TotalMass = Algo::Accumulate(ActorsOnPlate, 0.0f,
		[](float l, AActor* r) -> float {
		return l + r->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	});

	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float TotalMass = GetTotalMassOfActorsOnPlate();

	// Mass of chair or mass of default pawn should open the door.
	if (FMath::IsNearlyEqual(TotalMass, TriggerMassObjects, 0.5f)
		|| FMath::IsNearlyEqual(TotalMass, TriggerMassPawn, 0.5f))
	{
		OnOpenRequest.Broadcast();
	}
	else
	{
		OnCloseRequst.Broadcast();
	}
}
