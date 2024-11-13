// Fill out your copyright notice in the Description page of Project Settings.


#include "EventLight.h"
#include "Components/DirectionalLightComponent.h"

// Sets default values
AEventLight::AEventLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	EventDirectionLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Event Direction Light"));

}

void AEventLight::SetLightIntensity(float Intensity)
{
	if(EventDirectionLight)
	{
		EventDirectionLight->SetIntensity(Intensity);
	}
}

// Called when the game starts or when spawned
void AEventLight::BeginPlay()
{
	Super::BeginPlay();
	SetLightIntensity(1000);
	
}

// Called every frame
void AEventLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

