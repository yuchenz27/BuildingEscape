// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionReporter.h"
// This preprocessor directive cannot be written in the header file, which is very confused
#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UPositionReporter::UPositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReporter::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	FVector ObjectCoord = GetOwner()->GetTransform().GetLocation();
	UE_LOG(LogTemp, Warning, TEXT("The position of object %s is (%f, %f, %f)"), *ObjectName, ObjectCoord.X, ObjectCoord.Y, ObjectCoord.Z);
	
}


// Called every frame
void UPositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

