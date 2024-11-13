// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventLight.generated.h"

UCLASS()
class BEATEMUP_API AEventLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventLight();
	UPROPERTY(EditAnywhere)
	UDirectionalLightComponent* EventDirectionLight;

	UFUNCTION()
	void SetLightIntensity(float Intensity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
