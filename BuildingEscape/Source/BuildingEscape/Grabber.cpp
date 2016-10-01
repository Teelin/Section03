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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	SetPlayerViewport();

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineEnd());
	}
}

void UGrabber::Grab()
{
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab, 
			NAME_None, 
			ComponentToGrab->GetOwner()->GetActorLocation(), 
			true
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult HitData;
	FCollisionObjectQueryParams ObjectsToQuery = ECollisionChannel::ECC_PhysicsBody;
	FCollisionQueryParams CollisionParameters;
	GetWorld()->LineTraceSingleByObjectType(HitData, GetLineStart(), GetLineEnd(), ObjectsToQuery, CollisionParameters);
	
	return HitData;
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing Physics handle"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an Input Component"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetLineStart()
{
	return MyLocation;
}

FVector UGrabber::GetLineEnd()
{
	return GetLineStart() + MyRotation.Vector() * Reach;;
}

void UGrabber::SetPlayerViewport()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(MyLocation, MyRotation);
}

