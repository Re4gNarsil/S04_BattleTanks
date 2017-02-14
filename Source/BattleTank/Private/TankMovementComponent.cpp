// Copyright EmbraceIT Ltd.

#include "BattleTank.h"
#include "TankTrack.h"
#include "TankMovementComponent.h"

void UTankMovementComponent::InitialiseMovement(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }

	FVector OurDirection = GetOwner()->GetActorForwardVector();
	FVector IntendedDirection = MoveVelocity.GetSafeNormal();

	/// Calculate how much force to apply forwards and left/right based on where the player's tank is, then apply the forces
	float RightThrow = FVector::CrossProduct(IntendedDirection, OurDirection).Z;
	float ForwardThrow = FVector::DotProduct(IntendedDirection, OurDirection);

	if (LeftTrack->ComponentVelocity.IsNearlyZero()) { 
		LeftTrack->MoreThrottle();
		RightTrack->MoreThrottle();
	} else { 
		LeftTrack->ResetThrottle();
		RightTrack->ResetThrottle();
	}

	IntendTurnRight(RightThrow);
	IntendMoveForward(ForwardThrow);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}
