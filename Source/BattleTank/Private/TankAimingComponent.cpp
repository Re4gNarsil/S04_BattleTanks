// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::InitialiseAiming(UTankBarrel* Barrel, UTankTurret* Turret)
{
	OurBarrel = Barrel;
	OurTurret = Turret;
	LastFireTime = FPlatformTime::Seconds();
}

// This will return true if we are aimed at our target
bool UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(OurBarrel)) { return false; }

	FVector OutLaunchVelocity;
	FVector StartLocation = OurBarrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0, 0,
		ESuggestProjVelocityTraceOption::DoNotTrace // Paramater must be present to prevent bug
	);

	if (bHaveAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		if (!(((AimDirection - OurBarrel->GetForwardVector()).IsNearlyZero()))) { MoveBarrelTowards(AimDirection); }
		if (OurBarrel->GetForwardVector().Equals(AimDirection, .01f)) { AimStatus = EAimStatus::Locked; }
		else { AimStatus = EAimStatus::Aiming; }
	}
	//UE_LOG(LogTemp, Warning, TEXT("time is %f"), ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds));
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) { AimStatus = EAimStatus::Reloading; }
	if (AmmoSupply == 0) { AimStatus = EAimStatus::Empty; }
	if (AimStatus == EAimStatus::Locked) { return true; }
	return false;
}

/// Doesn't work unfortunately
FVector UTankAimingComponent::CurrentAimLocation(float Speed)
{
	FVector OutHitLocation = FVector(0);
	FVector StartLocation = OurBarrel->GetSocketLocation(FName("Projectile"));
	FVector LaunchVelocity = OurBarrel->GetForwardVector();

	UGameplayStatics::SuggestProjectileVelocity
	(
		this, LaunchVelocity, StartLocation, OutHitLocation, Speed, false, 0, 0,
		ESuggestProjVelocityTraceOption::DoNotTrace // Paramater must be present to prevent bug
	);
	return OutHitLocation;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(OurBarrel && OurTurret)) { return; }

	// Work-out difference between current barrel rotation, and AimDirection
	auto BarrelRotator = OurBarrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	if (FMath::Abs(DeltaRotator.Yaw) > 180.f) { DeltaRotator.Yaw -= (FMath::Clamp(DeltaRotator.Yaw, -1.f, 1.f) * 360.f); }
	OurBarrel->Elevate(DeltaRotator.Pitch);
	OurTurret->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	//UTankBarrel* OurBarrel = GetOwner()->FindComponentByClass<UTankBarrel>();
	if (!ensure(OurBarrel)) { return; }
	if (AimStatus != EAimStatus::Reloading && AimStatus != EAimStatus::Empty)
	{
		// Spawn a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			OurBarrel->GetSocketLocation(FName("Projectile")),
			OurBarrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		AmmoSupply--;
	}
}

int UTankAimingComponent::AmmoLeft()
{
	return AmmoSupply;
}
