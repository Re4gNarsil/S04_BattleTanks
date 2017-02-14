// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * TankTrack is used to set maximum driving force, and to apply forces to the tank.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Sets a throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float Throttle);
	
	// Max force per track, in Newtons; mass is compensated for elsewhere
	UPROPERTY(EditDefaultsOnly)
		//if you adjust this you may need to change Current/Boosted as well
		float TrackMaxDrivingForce = 600;
	float CurrentDrivingForce;
	float BoostedDrivingForce;

	void MoreThrottle();
	void ResetThrottle();

private:
	UTankTrack();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit);
	void ApplyForce(FVector Force);

	FVector ThrottleVelocity = FVector(0.f);
	
};
