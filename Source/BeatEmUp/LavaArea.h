// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LavaArea.generated.h"

UCLASS()
class BEATEMUP_API ALavaArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALavaArea();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* LavaMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* LavaClass;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* LavaMaterialInstance;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void DestroyArea();

	UPROPERTY(EditAnywhere)
	float AreaRemainTime = 10;

	FTimerHandle RemainCooldown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
