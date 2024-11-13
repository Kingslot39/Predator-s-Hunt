// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaArea.h"

#include "BeatEmUpCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ALavaArea::ALavaArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	LavaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lava Mesh"));
	RootComponent = LavaMesh;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	TriggerArea->SetupAttachment(RootComponent);
	TriggerArea->SetBoxExtent(FVector(50,50,10));
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ALavaArea::OnOverlapBegin);
	

}

void ALavaArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABeatEmUpCharacter* Character = Cast<ABeatEmUpCharacter>(OtherActor);
	if(Character)
	{
		Character->DealDamage(1000000);
	}
	GetWorld()->GetTimerManager().SetTimer(RemainCooldown, this, &ALavaArea::DestroyArea, AreaRemainTime, true);
}

void ALavaArea::DestroyArea()
{
	Destroy();
}

// Called when the game starts or when spawned
void ALavaArea::BeginPlay()
{
	Super::BeginPlay();
	LavaMaterialInstance = UMaterialInstanceDynamic::Create(LavaClass,this);
	LavaMesh->SetMaterial(0,LavaMaterialInstance);
	LavaMaterialInstance->SetScalarParameterValue(TEXT("Emissive"),0.7);
	LavaMaterialInstance->SetVectorParameterValue(TEXT("LavaColor"),FLinearColor::Yellow);
	
}

// Called every frame
void ALavaArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

