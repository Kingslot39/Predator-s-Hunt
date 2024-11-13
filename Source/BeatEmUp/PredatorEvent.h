// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventLight.h"
#include "EventUI.h"
#include "GameFramework/Actor.h"
#include "BeatEmUpCharacter.h"
#include "PredatorEvent.generated.h"

UCLASS()
class BEATEMUP_API APredatorEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredatorEvent();
	//UPROPERTY()
	//TArray<AEnemy*>Enemies;
	//UPROPERTY(EditAnywhere)
	//ABeatEmUpCharacter* Character;
    UPROPERTY(EditAnywhere)
	int EnemyDead;
	UPROPERTY(EditAnywhere)
	AEventLight* SkyLight;
	
	void EventStart();
	void EventEnd();

	FTimerHandle EventCooldown;
	bool isPredatorTime;
	UPROPERTY(EditAnywhere)
	int Seconds;

	UPROPERTY(EditAnywhere)
	float PlayerMaxATK;
	void CheckDeathEvent();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEventLight>LightClass;

	UPROPERTY(EditAnywhere)
	ABeatEmUpCharacter* Character;

	UPROPERTY(EditAnywhere)
	AEnemy* Enemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEventUI>EventUIClass;

	UPROPERTY(EditAnywhere)
	UEventUI* EventUI;

	void CooldownClock();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
