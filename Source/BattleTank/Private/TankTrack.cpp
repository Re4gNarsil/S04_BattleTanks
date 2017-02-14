// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTrack.h"

UTankTrack::UTankTrack() 
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
	ResetThrottle();
}

void UTankTrack::MoreThrottle()
{
	TrackMaxDrivingForce = BoostedDrivingForce;
}

void UTankTrack::ResetThrottle() {
	CurrentDrivingForce = TrackMaxDrivingForce;
	BoostedDrivingForce = TrackMaxDrivingForce * 1.2f;
}

void UTankTrack::SetThrottle(float Throttle)
{
	ThrottleVelocity += (GetForwardVector() * FMath::Clamp(Throttle, -1.F, 1.F) * TrackMaxDrivingForce);
}

void UTankTrack::ApplyForce(FVector Force) 
{
	auto Weight = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent())->GetMass();
	auto ForceLocation = GetComponentLocation();
	auto TrackRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	TrackRoot->AddForceAtLocation(Force * Weight, ForceLocation);
}

void UTankTrack::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit) 
{
	//calculate necessary side force to keep our tank going straight
	auto SlipSpeed = FVector::DotProduct(GetComponentVelocity(), GetRightVector());
	auto CorrectionVelocity = -(GetRightVector() * SlipSpeed / GetWorld()->DeltaTimeSeconds) / 2; //there are 2 tracks after all

	ApplyForce(CorrectionVelocity);
	ApplyForce(ThrottleVelocity);
	ThrottleVelocity = FVector(0.f);
}