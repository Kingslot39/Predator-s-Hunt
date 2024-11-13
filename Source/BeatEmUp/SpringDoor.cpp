// Fill out your copyright notice in the Description page of Project Settings.


#include "SpringDoor.h"

#include "Enemy.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"
#include "Runtime/GameMenuBuilder/Public/GameMenuItem.h"

// Sets default values
ASpringDoor::ASpringDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpringDoorMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Component"));
	RootComponent = SpringDoorMesh;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	TriggerArea->SetupAttachment(RootComponent);
	TriggerArea->SetBoxExtent(FVector(50,50,10));
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ASpringDoor::OnOverlapBegin);

}


void ASpringDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if(Enemy)
	{
		FVector LaunchDirection = FVector(500,300,3000);
		Enemy->LaunchCharacter(LaunchDirection,true,true);
	}
}

// Called when the game starts or when spawned
void ASpringDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpringDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

