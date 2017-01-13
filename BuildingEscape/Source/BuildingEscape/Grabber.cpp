// Fill out your copyright notice in the Description page of Project Settings.
#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	//Look for attached physics handle
	FindPhysicsHandleComponent();
	SetUpInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (inputComponent)
	{
		//Bind the input axis
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component missing!"));
	}
}

void UGrabber::Grab()
{
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();
	if (!physicsHandle) { return; }
	//Try and reach any actors with physics body collision channel set
	if (actorHit)
	{
		physicsHandle->GrabComponent(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	if (!physicsHandle) { return; }
	physicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	if (!physicsHandle) { return; }
	//If physics handle is attached
	if (physicsHandle->GrabbedComponent)
	{
		//Move the object that we're holding
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}
		}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//hit is the object hit by the line trace
	FHitResult hit;

	//Setup query parameter
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT hit, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);
	
	return hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	//Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s\n Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());

	//Draw the ray cast to show facing direction
	return playerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	//Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s\n Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());

	//Draw the ray cast to show facing direction
	return playerViewPointLocation + (playerViewPointRotation.Vector() * reach);
}


