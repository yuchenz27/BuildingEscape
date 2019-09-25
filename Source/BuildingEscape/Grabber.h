// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"

#define OUT

struct PlayerLineTraceInfo {
	FVector PlayerViewPointLocation;
	FVector LineTraceEnd;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// the maximal distance to grab an item
	UPROPERTY(EditAnywhere)
	float Reach = 140.f;
	// the handle used to move physics objects in the scene
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	// the pointer pointing to the input component generated at runtime
	UInputComponent* InputComponent = nullptr;
	
	void SetUpPhysicsHandle();
	void SetUpInputComponent();
	void Grab();
	void Release();
	FHitResult GetFirstPhysicsBodyInReach() const;
	void ComputePlayerLineTraceInfo(PlayerLineTraceInfo &Result) const;
};
