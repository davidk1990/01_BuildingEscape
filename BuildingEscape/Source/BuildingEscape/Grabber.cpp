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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
	//Look for attached physics handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has physics handle component"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}


	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (inputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found!"));
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
	//Grabs the object
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));
	return;
}

void UGrabber::Release()
{
	//Releases the object
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
	return;
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Get player viewpoint
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	//Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s\n Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());

	//Draw the ray cast to show facing direction
	FVector lineTraceEnd = playerViewPointLocation + (playerViewPointRotation.Vector() * reach);

	DrawDebugLine(GetWorld(), playerViewPointLocation, lineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 5.f);

	//Setup query parameter
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	
	//Ray-cast out to reach distance
	FHitResult hit;
	
	GetWorld()->LineTraceSingleByObjectType(OUT hit, playerViewPointLocation, lineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);

	

	//See what we hit
	AActor* actorHit = hit.GetActor();
	if (actorHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(actorHit->GetName()));
	}
}

