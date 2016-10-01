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

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing Physics handle"), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component has been found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an Input Component"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	FVector MyLocation;
	FRotator MyRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(MyLocation,MyRotation);
	Trace(MyLocation, MyRotation);
}

void UGrabber::Trace(FVector MyLocation, FRotator MyRotation)
{
	FVector Start = MyLocation;
	FVector End = Start + MyRotation.Vector() * Reach;

	FHitResult HitData;
	FCollisionObjectQueryParams ObjectsToQuery = ECollisionChannel::ECC_PhysicsBody;
	FCollisionQueryParams CollisionParameters;
	

	if (GetWorld()->LineTraceSingleByObjectType(HitData, Start, End, ObjectsToQuery,CollisionParameters)) 
	{
		AActor *HitActor = HitData.GetActor();

		UE_LOG(LogTemp, Warning, TEXT("%s"), *HitActor->GetName());
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0, 0, 1.f);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
}
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
}
