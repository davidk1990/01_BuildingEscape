// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:	
	//How far ahead of the player can we reach
	float reach = 100.f;

	UPhysicsHandleComponent* physicsHandle = nullptr;

	UInputComponent* inputComponent = nullptr;

	//Ray-cast and grab what's in reach
	void Grab();

	//Release the object you grabbed
	void Release();
	
	//Finds and gets the physics handle component
	void FindPhysicsHandleComponent();

	//Set up attached input component
	void SetUpInputComponent();
	
	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineStart();
	
	//Returns current end of the reach line
	FVector GetReachLineEnd();

};
