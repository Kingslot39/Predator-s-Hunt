// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBTController.h"
#include "Enemy.h"




AEnemyBTController::AEnemyBTController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
}

void AEnemyBTController::BeginPlay()
{
	Super::BeginPlay();
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBTController::OnSensesUpdate);
	BlackboardComponent->SetValueAsBool("HasAmmo", true);
	
	
}

void AEnemyBTController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); 
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	if(TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
	if(Enemy->CurrentHealth < 0.6 * Enemy->MaxHealth)
	{
		BlackboardComponent->SetValueAsBool("isLowHealth", true);
	}
	
}

FRotator AEnemyBTController::GetControlRotation() const
{
	if(TargetPlayer)
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw,0);
	}
	return Super::GetControlRotation();
}

void AEnemyBTController::GenerateNewRandomLocation()
{
	if(NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), PatrolDistance, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AEnemyBTController::OnSensesUpdate(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* SensedPawn = Cast<APawn>(UpdatedActor);
	if(SensedPawn)
	{
		if(SensedPawn->IsPlayerControlled())
		{
			if(Stimulus.WasSuccessfullySensed())
			{
				TargetPlayer = SensedPawn;
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
			}
			else
			{
				TargetPlayer = nullptr;
				BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			}
		}
	}
}
		
void AEnemyBTController::RageStatusUpdate()
{
	  Cast<AEnemy>(GetPawn())->StarRaging();
  	  BlackboardComponent->SetValueAsBool("Raging", true);
}


void AEnemyBTController::Shoot()
{
	FVector ShootDirection = TargetPlayer->GetActorLocation() - GetPawn()->GetActorLocation();
	Cast<AEnemy>(GetPawn())->Shoot(ShootDirection);
	Ammo--;
	BlackboardComponent->SetValueAsBool("HasAmmo", Ammo > 0);
}
