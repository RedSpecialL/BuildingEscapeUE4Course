// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Finds (assumed) attached physics handle component.
	void FindPhysicsHandleComponent();
	// Return hit for first physics body in reach.
	const FHitResult GetFirstPhysicsBodyInReach() const;
	// Ray-cast and grab what's it reach.
	void Grab();
	// Called when grab is released.
	void GrabReleased();
	// Setup (assumed) attached input component.
	void SetupInputComponent();



private:
	UInputComponent* InputComponent = nullptr;

	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// How far ahead of the player can we reach in centimeters.
	float ReachCm = 100.0f;
};
