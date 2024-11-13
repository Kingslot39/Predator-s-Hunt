// Fill out your copyright notice in the Description page of Project Settings.

#include "Shuriken.h"
#include "Enemy.h"


// Sets default values
AShuriken::AShuriken()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));

	MovementComponent->bShouldBounce = true;
	MovementComponent->BounceVelocityStopSimulatingThreshold = MovementSpeed / 2;
	MovementComponent->bSweepCollision = true;
	MovementComponent->InitialSpeed = MovementSpeed;
	MovementComponent->UpdatedComponent = Mesh;
	MovementComponent->ProjectileGravityScale = 0;

}

void AShuriken::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	if(OtherActor && OtherActor != this)
	{
		AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
		if(HitEnemy)
		{
			HitEnemy->DealDamage(Damage);
			Destroy();
		}
		
	}
}


// Called when the game starts or when spawned
void AShuriken::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &AShuriken::OnHit);
	Mesh->SetNotifyRigidBodyCollision(true);
	
}

// Called every frame
void AShuriken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

