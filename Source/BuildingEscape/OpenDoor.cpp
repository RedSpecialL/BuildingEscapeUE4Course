// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/TriggerVolume.h"
#include "Components/PrimitiveComponent.h"
#include "Algo/Accumulate.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
	: LastDoorOpenTime(0)
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
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation.
	Owner->SetActorRotation(FRotator(0.0f, CloseAngle, 0.0f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	TArray<AActor*> ActorsOnPlate;
	PressurePlate->GetOverlappingActors(OUT ActorsOnPlate);
	//ActorsOnPlate[0]->FindComponentByClass
	float TotalMass = Algo::Accumulate(ActorsOnPlate, 0.0f,
		[](float l, AActor* r) -> float {
		return l + r->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	});

	UE_LOG(LogTemp, Warning, TEXT("Total mass in the plate %f"), TotalMass)
	UE_LOG(LogTemp, Warning, TEXT("Number of objects %d"), ActorsOnPlate.Num())
	
	return TotalMass;

// 	for (AActor* Actor : ActorsOnPlate)
// 	{
// 		Algo::Accu
// 		Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
// 	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Poll the Trigger Volume
	/// If the ActorThatOpens is in the volume
	const float TotalMass = GetTotalMassOfActorsOnPlate();

	// Mass of chair or mass of default pawn.
	// TODO: Remove hardcode!
	if (FMath::IsNearlyEqual(TotalMass, 10.0f, 0.5f) || FMath::IsNearlyEqual(TotalMass, 100.0f, 0.5f))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

