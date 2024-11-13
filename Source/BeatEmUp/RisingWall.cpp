// Fill out your copyright notice in the Description page of Project Settings.


#include "RisingWall.h"

#include "Enemy.h"
#include "Chaos/ChaosPerfTest.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "Misc/TextFilterExpressionEvaluator.h"

// Sets default values
// This is a Rising Wall code but I put the wrong name on it.
ARisingWall::ARisingWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree Mesh"));
	RootComponent = TreeMesh;


	

	WallReadytoRise = true;

}



// Called when the game starts or when spawned
void ARisingWall::BeginPlay()
{
	Super::BeginPlay();
	InitialPosition = GetActorLocation();
	TargetPosition = InitialPosition + FVector(0,0, 300);
}

// Called every frame
void ARisingWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ARisingWall::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	//set the wall up
	if(WallReadytoRise)
	{
		SetActorLocation(TargetPosition);
		GetWorld()->GetTimerManager().SetTimer(RisingCooldown, this, &ARisingWall::SetBackPosition, Seconds, false);
	
		// set up the color for the wall (isRising)
		WallisRising = UMaterialInstanceDynamic::Create(WallRise,this);
		TreeMesh->SetMaterial(0,WallisRising);
		WallisRising->SetVectorParameterValue("RisingColor",FLinearColor::Green);
		WallisRising->SetScalarParameterValue("EmessiveStrength", 3);
	}
	
	
	
    	
}

void ARisingWall::SetBackPosition()
{
	
  	// set the wall down
	WallReadytoRise = false;
  	FVector PastLocation = GetActorLocation() - FVector(0,0,300);
  	SetActorLocation(PastLocation);
    
  	// set up the color for the wall (cooldown)
	WallisCooldown = UMaterialInstanceDynamic::Create(WallCooldown,this);
	TreeMesh->SetMaterial(0,WallisCooldown);
	WallisCooldown->SetVectorParameterValue("CooldownColor", FLinearColor::Red);
	WallisCooldown->SetScalarParameterValue("Emissive",1);
	GetWorld()->GetTimerManager().SetTimer(RisingCooldown, this, &ARisingWall::SetWallReady, 5.f, false);
	
}

void ARisingWall::SetWallReady()
{
	WallReadytoRise = true;
	WallisReady = UMaterialInstanceDynamic::Create(WallToRise,this);
	TreeMesh->SetMaterial(0,WallisReady);
	WallisReady->SetVectorParameterValue("ReadyColor", FLinearColor::White);
	WallisReady->SetScalarParameterValue("Emissive",2);
	
}








