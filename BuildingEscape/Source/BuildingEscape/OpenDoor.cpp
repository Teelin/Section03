// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("No Trigger Volume on %s"),*Owner->GetName());
	}
	
}
// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	if (GetTotalMassOnPressurePlate()>ThresholdMass)
	{
		OnOpenRequest.Broadcast();
	}
	else
	{
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOnPressurePlate()
{
	float TotalMass = 0.f;
	TArray <AActor*> ActorsOnPressurePlate;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(ActorsOnPressurePlate);
	for (const auto& actor : ActorsOnPressurePlate) 
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

