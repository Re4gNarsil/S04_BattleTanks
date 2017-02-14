// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collider Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);

	/// Create and attach all necessary componets

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepWorldTransform);
	ImpactBlast->bAutoActivate = false;

	ImpactImpulse = CreateDefaultSubobject<URadialForceComponent>(FName("Impact Pulse"));
	ImpactImpulse->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepWorldTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

void AProjectile::EmitImpactExplosion() 
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ImpactImpulse->FireImpulse();
	SetRootComponent(ImpactImpulse);
	
	UGameplayStatics::ApplyRadialDamage(this, ProjectileDamage, GetActorLocation(), ImpactImpulse->Radius, UDamageType::StaticClass(), TArray<AActor*>());

	CollisionMesh->DestroyComponent();
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::OnTimeExpires, DestroyDelay, false, -1.f);
}

void AProjectile::OnTimeExpires() 
{
	Destroy();
}