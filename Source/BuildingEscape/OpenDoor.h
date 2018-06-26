// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenDoorRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	
public:
	// Closes the door.
	void CloseDoor();
	// Opens the door.
	void OpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnOpenDoorRequest OnOpenRequest;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Returns total mass of actors on the pressure plate.
	float GetTotalMassOfActorsOnPlate() const;

private:
	// Angle in which door is opened.
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -165.0f;
	// Angle in which door is closed.
	UPROPERTY(VisibleAnywhere)
	float CloseAngle = -90.0f;
	// Trigger Volume that triggers door opening.
	UPROPERTY(EditAnywhere)
	class ATriggerVolume* PressurePlate = nullptr;
	// Delay with which door will be closed.
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.85f;
	// Latest opening time.
	float LastDoorOpenTime;
	// The owning door.
	AActor* Owner = nullptr;
};
