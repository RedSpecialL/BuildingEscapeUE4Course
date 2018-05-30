// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	// Closes the door.
	void CloseDoor();
	// Opens the door.
	void OpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -150.0f;

	//
	UPROPERTY(VisibleAnywhere)
	float CloseAngle = -90.0f;

	//
	UPROPERTY(EditAnywhere)
	class ATriggerVolume* PressurePlate;

	//
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.85f;

	float LastDoorOpenTime;

	AActor* ActorThatOpens;
	// The owning door.
	AActor* Owner;
};