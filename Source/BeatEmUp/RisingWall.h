// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RisingWall.generated.h"

UCLASS()
class BEATEMUP_API ARisingWall : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARisingWall();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TreeMesh;
	
	virtual void Interact_Implementation() override;
	FTimerHandle RisingCooldown;
	UPROPERTY(EditAnywhere)
	float Seconds = 5;
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* WallisRising;
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* WallisCooldown;
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* WallisReady;
    UPROPERTY(EditAnywhere)
	UMaterialInterface* WallRise;

	bool WallReadytoRise;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* WallCooldown;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* WallToRise;
	
    UPROPERTY(EditAnywhere)
	float CoolingDownSeconds = 10;
	
	void SetBackPosition();

	void SetWallReady();
	

	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
	FVector InitialPosition;
	FVector TargetPosition;
	bool isReadyRising;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
