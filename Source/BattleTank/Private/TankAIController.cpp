// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { UE_LOG(LogTemp, Warning, TEXT("Aiming Component Not Found on AI Controller")); }
}

void ATankAIController::SetPawn(APawn* InPawn) 
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		ATank* OurTank = Cast<ATank>(InPawn);

		if (!ensure(OurTank)) { return; }
		OurTank->TankIsDestroyed.AddDynamic(this, &ATankAIController::WhenTankIsDestroyed);
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(AimingComponent)) { return; }
	if (PlayerTank)
	{
		//Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards the player
		// This will return true if we are aimed at our target
		if (AimingComponent->AimAt(PlayerTank->GetActorLocation())) { AimingComponent->Fire(); }
	}
}

void ATankAIController::WhenTankIsDestroyed()
{
	if (GetPawn()) {
		GetPawn()->DetachFromControllerPendingDestroy();
	}
}
