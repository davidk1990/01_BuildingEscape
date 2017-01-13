// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	owner = GetOwner();

	if (!pressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing pressure plate."), *GetOwner()->GetName());
	}

}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (GetMassOfActors() > triggerMass)
	{
		OnOpen.Broadcast();
	}else
	{
		OnClose.Broadcast();
	}
		
}

float UOpenDoor::GetMassOfActors()
{
	float totalMass = 0.f;

	TArray<AActor*> overlappingActors;
	if (!pressurePlate) { return totalMass; }
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	for (const auto& AActor : overlappingActors)
	{
		totalMass += AActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on a pressure plate"), *AActor->GetName())
	}
	
	return totalMass;
}

