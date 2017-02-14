// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TankAIController.generated.h"

//Forward declarations
class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void WhenTankIsDestroyed();


protected:
	UTankAimingComponent* AimingComponent = nullptr;

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetPawn(APawn* Inpawn) override;
	
	// How close can the AI tank get
	//UPROPERTY(EditAnywhere, Category = "Setup")
	float AcceptanceRadius = 20000;
};
