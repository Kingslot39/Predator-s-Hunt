// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeatEmUpCharacter.h"
#include "Bullet.h"
#include "DamageTextUI.h"
#include "DynamicSpotlight.h"
#include "HealthPack.h"
#include "LavaArea.h"
#include "PredatorEvent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class BEATEMUP_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
    UPROPERTY()
	ABeatEmUpCharacter* Player;
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere)
	float CurrentHealth = MaxHealth;

	void DealDamage(float Damage);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float DamageToPlayer = 10;
	UPROPERTY(EditAnywhere)
	float RagdollTime = 5;
	FTimerHandle RagdollTimerHandle;
	void Ragdoll();
	UFUNCTION()
	void StopRagdoll();

	void StarRaging();
	
	
   UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> BulletClass;
	void Shoot(FVector Direction);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageTextUI> DamageTextClass;
	
    UPROPERTY(EditAnywhere)
	int EXPAmount = 20;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathParticleClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHealthPack>HealthPackClass;

	UPROPERTY(EditAnywhere)
     TSubclassOf<ADynamicSpotlight> DynamicSpotlightClass;
	UPROPERTY(EditAnywhere)
	ADynamicSpotlight* Spotlight;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APredatorEvent>EventClass;
	
	UPROPERTY()
	APredatorEvent* PredatorEvent;
	



	UFUNCTION()
    void EventBegin();
	
    void EventEnd();
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ALavaArea>LavaClass;

	void CreateLavaArea();

	FTimerHandle SpawnTimerHandle;

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
