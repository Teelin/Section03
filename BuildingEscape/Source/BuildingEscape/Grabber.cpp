// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	FVector MyLocation;
	FRotator MyRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(MyLocation,MyRotation);
	//UE_LOG(LogTemp, Warning, TEXT("My Location is %s and my Rotation is %s"), *MyLocation.ToString(), *MyRotation.ToString());
	Trace(MyLocation, MyRotation);
}

void UGrabber::Trace(FVector MyLocation, FRotator MyRotation)
{
	FVector Start = MyLocation;
	FVector End = Start + MyRotation.Vector() * Reach;

	FHitResult HitData(ForceInit);
	FCollisionObjectQueryParams ObjectsToQuery = ECollisionChannel::ECC_PhysicsBody;
	FCollisionQueryParams CollisionParameters;
	

	if (GetWorld()->LineTraceSingleByObjectType(HitData, Start, End, ObjectsToQuery,CollisionParameters)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Dynamic"));
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0, 0, 1.f);
}