// Fill out your copyright notice in the Description page of Project Settings.

#include "PredatorEvent.h"
#include "Enemy.h"
#include "BeatEmUpCharacter.h"
#include "EventUI.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APredatorEvent::APredatorEvent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Seconds = 20;

}

void APredatorEvent::EventStart()
{
	
	// Set up for UI in the Event;
		EventUI->EventSecondCooldown->SetText(FText::FromString(FString::FromInt(Seconds)));
		EventUI->AddToViewport();
		// set the intensity of Direction light to 0 -> dark scene
		TArray<AActor*> LightActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEventLight::StaticClass(), LightActor );
		for(AActor* Actor : LightActor)
		{
			SkyLight = Cast<AEventLight>(Actor);
			if(SkyLight)
			{
				SkyLight->SetLightIntensity(0);
			}
		}
		// Call the Enemy around the map
		UWorld* World = GetWorld();
		TArray<AActor*> MyActor;
		UGameplayStatics::GetAllActorsOfClass(World, AEnemy::StaticClass(), MyActor );
		for(AActor* Actor : MyActor)
		{
			AEnemy* EnemyActor = Cast<AEnemy>(Actor);
			EnemyActor->EventBegin();
		}
		//return the player
		if(Character)
		{
			Character->PunchDamage = 0;
			Character->RewindDamage = 0;
		}
	GetWorldTimerManager().SetTimer(EventCooldown,this,&APredatorEvent::CooldownClock, 1.f,true);
	
	
}


	
	


void APredatorEvent::EventEnd()
{
		EventUI->RemoveFromViewport();
		TArray<AActor*> LightActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEventLight::StaticClass(), LightActor );
		for(AActor* Actor : LightActor)
		{
			SkyLight = Cast<AEventLight>(Actor);
			if(SkyLight)
			{
				SkyLight->SetLightIntensity(1000);
			}
		}
		
		TArray<AActor*> MyActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), MyActor );
		for(AActor* Actor : MyActor)
		{
			AEnemy* EnemyActor = Cast<AEnemy>(Actor);
			EnemyActor->EventEnd();
		}
		if(Character)
		{
			Character->PunchDamage = PlayerMaxATK;
			Character->RewindDamage = 75;
		}
	
		
	
}

void APredatorEvent::CheckDeathEvent()
{
	EnemyDead = Character->EnemyKills;
	if(EnemyDead >= 2)
	{
		EventStart();
	}
}

void APredatorEvent::CooldownClock()
{
	Seconds -= 1;
	EventUI->EventSecondCooldown->SetText(FText::FromString(FString::FromInt(Seconds)));
	EventUI->AddToViewport();
	if(Seconds <= 0)
	{
		EventEnd();
	}
}

// Called when the game starts or when spawned
void APredatorEvent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	EventUI = Cast<UEventUI>(CreateWidget(GetGameInstance(),EventUIClass));	
	PlayerMaxATK = Character->MaxATK;
	
}

// Called every frame
void APredatorEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
   
}
