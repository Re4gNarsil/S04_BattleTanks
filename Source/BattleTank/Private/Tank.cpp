// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	CurrentTankHealth = StartingTankHealth;
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	CurrentTankHealth -= FMath::Clamp(DamageAmount, 0.f, CurrentTankHealth);
	if (CurrentTankHealth < .001f) { 
		TankIsDestroyed.Broadcast();
	}
	return CurrentTankHealth;
}

float ATank::TankHealthLeft() 
{
	return (CurrentTankHealth / StartingTankHealth);
}

