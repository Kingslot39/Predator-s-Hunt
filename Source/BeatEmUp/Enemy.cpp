// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "BrainComponent.h"
#include "EnemyBTController.h"
#include "EnemyController.h"
#include "NiagaraComponent.h"
#include "PredatorEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	


}

void AEnemy::DealDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	Player->ComboCounter++;
	UDamageTextUI* SpawnedDamage = Cast<UDamageTextUI>(CreateWidget(GetGameInstance(),DamageTextClass));
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), GetMesh()->GetComponentLocation(),SpawnedDamage->CurrentLocation);
    if(SpawnedDamage)
    {
    	SpawnedDamage->DamageText->SetText(FText::FromString(FString::SanitizeFloat(Damage)));
    	SpawnedDamage->TargetLocation += SpawnedDamage->CurrentLocation;
    	SpawnedDamage->AddToViewport();
    }
	

}


void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	if (OtherActor == Player)
	{
		Player->DealDamage(DamageToPlayer);
		Destroy();
	}
}

void AEnemy::Ragdoll()
{
	Cast<AEnemyBTController>(GetController())->BrainComponent->PauseLogic("Ragdolling!");
	GetMesh()->SetCollisionProfileName("Ragdoll");
    GetMesh()->SetSimulatePhysics(true);
    GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
    GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AEnemy::StopRagdoll, RagdollTime, false);

}

void AEnemy::StopRagdoll()
{
	Cast<AEnemyBTController>(GetController())->BrainComponent->ResumeLogic("Moving Again!");
	if(CurrentHealth <= 0)
	{
		
		Player->AddEXP(EXPAmount);
		if(DeathParticleClass)
		{
			UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),DeathParticleClass, GetMesh()->GetComponentLocation());
			SpawnedEffect->SetColorParameter(FName("User.SpawnColour"),FLinearColor::MakeRandomColor());
		}
		if(HealthPackClass)
		{
			// Spawn location of the Health Pack
			FVector SpawnLocation = GetMesh()->GetComponentLocation()+FVector(0,0,50);
			GetWorld()->SpawnActor(HealthPackClass, &SpawnLocation);
		}
		Player->EnemyKills++;
		if(PredatorEvent)
		{
			PredatorEvent->CheckDeathEvent();
		}
		Spotlight->Destroy();
		Destroy();
		return;
	}
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("pelvis"));
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	GetCharacterMovement()->GravityScale = 1;


}

void AEnemy::StarRaging()
{
	GetCharacterMovement()->MaxWalkSpeed+= 500.f;
	DamageToPlayer = 30;
}




void AEnemy::Shoot(FVector Direction)
{
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100;
	FRotator SpawnRotation = Direction.Rotation();
	ABullet* SpawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(BulletClass, &SpawnLocation, &SpawnRotation));
	Direction.Normalize();
	SpawnedBullet->MovementComponent->Velocity = Direction * SpawnedBullet->MovementSpeed;
	
}



void AEnemy::EventBegin()

{
	GetCharacterMovement()->MaxWalkSpeed+= 800.f;
	Player->PunchDamage = 0;
	Player->RewindDamage = 0;
	
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemy::CreateLavaArea, 3.0f, false );
	
}

void AEnemy::EventEnd()
{
	
	GetCharacterMovement()->MaxWalkSpeed-= 500.f;
	this->DamageToPlayer = 10;
	Player->PunchDamage = 50;
	Player->RewindDamage = 75;
}


void AEnemy::CreateLavaArea()
{
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z-100);
	FRotator Rotation = GetActorRotation();
	GetWorld()->SpawnActor(LavaClass, &Location, &Rotation);
}



// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);
	if(DynamicSpotlightClass)
	{
		FVector SpawnLocation = GetActorLocation();
		Spotlight = Cast<ADynamicSpotlight>(GetWorld()->SpawnActor(DynamicSpotlightClass, &SpawnLocation));
		Spotlight->TargetActor = this;
	}
	PredatorEvent = GetWorld()->SpawnActor<APredatorEvent>(EventClass);

	

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

