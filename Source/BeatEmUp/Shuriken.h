// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Shuriken.generated.h"

UCLASS()
class BEATEMUP_API AShuriken : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShuriken();
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MovementComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 2000;
	UPROPERTY(EditAnywhere)
	float Damage = 25;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
