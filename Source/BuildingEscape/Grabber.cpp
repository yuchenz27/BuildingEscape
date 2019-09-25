// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SetUpPhysicsHandle();
	SetUpInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// translate the object we are holding in every frame
	if (PhysicsHandle->GrabbedComponent != nullptr) {
		PlayerLineTraceInfo ThisPlayerLineTraceInfo;
		ComputePlayerLineTraceInfo(OUT ThisPlayerLineTraceInfo);
		PhysicsHandle->SetTargetLocation(ThisPlayerLineTraceInfo.LineTraceEnd);
	}
	
}

// set up the physics handle at the beginning of the game
void UGrabber::SetUpPhysicsHandle()
{
	// look for the attached physics handle of the owner
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s failed to set up the physics handle."), *GetOwner()->GetName());
	}
}

// set up the input component at the beginning of the game
void UGrabber::SetUpInputComponent()
{
	// look for the generated input component attached to the owner
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s failed to set up the input component."), *GetOwner()->GetName());
		return;
	}
	
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	
}

// grab the item reached by line trace
void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	// Do the line trace
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();  // get the mesh

	if (ComponentToGrab != nullptr) {
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	PlayerLineTraceInfo ThisPlayerLineTraceInfo;
	ComputePlayerLineTraceInfo(OUT ThisPlayerLineTraceInfo);

	/// Draw the line trace debug line
	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 6.f);

	/// Line-trace (ray-casting)
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());  // this false indicates we are using the bounding box instead of the complex mesh
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, ThisPlayerLineTraceInfo.PlayerViewPointLocation, ThisPlayerLineTraceInfo.LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	
	return HitResult;
}

// compute the current line trace info based on the current player location and facing direction
void UGrabber::ComputePlayerLineTraceInfo(PlayerLineTraceInfo &Result) const
{

	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT Result.PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	/// compute the line trace end point
	Result.LineTraceEnd = Result.PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}