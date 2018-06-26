// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDoorRequest OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FOnDoorRequest OnCloseRequst;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Returns total mass of actors on the pressure plate.
	float GetTotalMassOfActorsOnPlate() const;

private:
	// Trigger Volume that triggers door opening.
	UPROPERTY(EditAnywhere)
	class ATriggerVolume* PressurePlate = nullptr;
	// The owning door.
	AActor* Owner = nullptr;
	// Mass on the plate that will open the door.
	UPROPERTY(EditAnywhere)
	float TriggerMassPawn = 100.0f;
	// Mass on the plate that will open the door.
	UPROPERTY(EditAnywhere)
	float TriggerMassObjects = 10.0f;
};
