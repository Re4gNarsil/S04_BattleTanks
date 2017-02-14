// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//Enum class for aim status
UENUM()
enum class EAimStatus : uint8
{
	Reloading,
	Aiming,
	Locked,
	Empty
};

// Forward Declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;

// Holds barrel's properties and Elevate method
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
		void InitialiseAiming(UTankBarrel* Barrel, UTankTurret* Turret);

	// Sets default values for this component's properties
	UTankAimingComponent();

	//this will return true if we are aiming directly at an enemy
	bool AimAt(FVector HitLocation);

	UPROPERTY(BlueprintReadOnly, Category = "State")
		EAimStatus AimStatus = EAimStatus::Aiming;

	UFUNCTION(BlueprintCallable, Category = "Firing")
		void Fire();
	UFUNCTION(BlueprintCallable, Category = "Firing")
		int AmmoLeft();

private:
	UTankBarrel* OurBarrel = nullptr;
	UTankTurret* OurTurret = nullptr;

	void MoveBarrelTowards(FVector AimDirection);
	FVector CurrentAimLocation(float LaunchSpeed);
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 6000;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float ReloadTimeInSeconds = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		int AmmoSupply = 10;
	double LastFireTime;

};
