// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeatEmUpCharacter.h"
#include "Enemy.h"
#include "NiagaraComponent.h"
#include "Interactable.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Landmark.h"
#include "PauseMenu.h"
#include "Shuriken.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABeatEmUpCharacter

ABeatEmUpCharacter::ABeatEmUpCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ABeatEmUpCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	InGameUI = Cast<UInGameUI>(CreateWidget(GetGameInstance(), InGameUIClass));
	InGameUI->Player = this;
	InGameUI->UpdateValues();
	InGameUI->AddToViewport();
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABeatEmUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::Look);

		//Punch
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::Punch);
		//Use
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::Use);

		//Set Up
		EnhancedInputComponent->BindAction(SetUpAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::SetUp);

		//Rewind
		EnhancedInputComponent->BindAction(RewindAction,ETriggerEvent::Started, this, &ABeatEmUpCharacter::Rewind);

		//Shooting
		EnhancedInputComponent->BindAction(ShootingAction,ETriggerEvent::Started, this, &ABeatEmUpCharacter::Shooting );

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Started, this, &ABeatEmUpCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Completed, this, &ABeatEmUpCharacter::StopSprint);

		//Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::PauseGame);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
// Use function to activate the object inside the map
void ABeatEmUpCharacter::Use()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetFollowCamera()->GetForwardVector() * UseDistance;
	FHitResult HitData;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	TraceParams.TraceTag = FName("Use Trace Tag");
	GetWorld()->DebugDrawTraceTag = TraceParams.TraceTag;

	bool bSweep = GetWorld()->LineTraceSingleByChannel(HitData, Start, End, ECC_Visibility, TraceParams);
	
	if(bSweep)
	{
		if(HitData.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("We hit: %s"), *HitData.GetActor()->GetName());
			if(HitData.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				IInteractable::Execute_Interact(HitData.GetActor());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor not interactable!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("What happened!?"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't hit an actor!"));
	}



	
}

void ABeatEmUpCharacter::Shooting()
{
	
		FVector Direction = GetActorLocation();
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100;
		FRotator SpawnRotation = Direction.Rotation();
		AShuriken* SpawnedBullet = Cast<AShuriken>(GetWorld()->SpawnActor(ShurikenClass, &SpawnLocation, &SpawnRotation));
		Direction.Normalize();
		SpawnedBullet->MovementComponent->Velocity = Direction * SpawnedBullet->MovementSpeed;
	
}


// Tick function use to update the UI value 
void ABeatEmUpCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(GetWorld()->GetTimerManager().TimerExists(PunchTimerHandle))
	{
		InGameUI->UpdateValues();
	}
	
}



void ABeatEmUpCharacter::AddEXP(int EXPToAdd)
{
	CurrentEXP += EXPToAdd;
	
	while(CurrentEXP >EXPToLevel)
	{
		CurrentEXP -= EXPToLevel;
		EXPToLevel *= IncreaseMultiplier;
		PunchDamage *= IncreaseMultiplier;
		MaxHealth *= IncreaseMultiplier;
		CurrentHealth = MaxHealth;
		MaxATK = PunchDamage;
	}
	InGameUI->UpdateValues();
}

//Punching action
void ABeatEmUpCharacter::Punch()
{
	if(bPunchReady)
	{
		if(PunchEffectClass)
		{
			UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PunchEffectClass, GetActorLocation(), GetActorRotation());
			SpawnedEffect->SetColorParameter(FName("User.SpawnColour"), FLinearColor::MakeRandomColor());
		}
		bPunchReady = false;
		GetWorld()->GetTimerManager().SetTimer(PunchTimerHandle,this,&ABeatEmUpCharacter::ResetPunch, PunchCoolDown, false);
		TArray<FHitResult> HitResults;
		const FVector Start = GetActorLocation();
		const FVector End = Start + GetActorForwardVector() * PunchDistance;
		const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(PunchDistance));
		const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, GetActorQuat(), ECC_WorldDynamic, CubeShape);

		TArray<AActor*> HitThisPunch;
		for(FHitResult HitResult : HitResults)
		{
			if(HitResult.GetActor() != this && !HitThisPunch.Contains(HitResult.GetActor()))
			{
				HitThisPunch.Add(HitResult.GetActor());
				AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor());
				
				if(HitEnemy)
				{
					HitEnemy->Ragdoll();
					FVector LaunchDirection = HitEnemy->GetActorLocation() - GetActorLocation();
					LaunchDirection.Normalize();
					LaunchDirection *= 3;
					LaunchDirection += FVector::UpVector;
					HitEnemy->GetMesh()->AddImpulse(LaunchDirection * PunchForce);
					HitEnemy->DealDamage(PunchDamage);
				}
			}
		}
	}
}


void ABeatEmUpCharacter::ResetPunch()
{
	bPunchReady = true;
	InGameUI->UpdateValues();
}

void ABeatEmUpCharacter::DealDamage(float Damage)
{
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	ComboCounter = 0;
	InGameUI->UpdateValues();
	if(CurrentHealth == 0)
	{
		Cast<APlayerController>(GetController())->RestartLevel();
	}
}

// Set up the landmark for rewind
void ABeatEmUpCharacter::SetUp()
{
   if(bAlreadySetUp == false)
   {
   	
   	 FVector Location = GetActorLocation();
   	 SpawnTeleport = (GetWorld()->SpawnActor(LandmarkClass, &Location));
   	 bAlreadySetUp = true;
   	 bCanRewind = true;
   }
	   
	
}

// Sprinting
void ABeatEmUpCharacter::StartSprint()
{
	IsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed+=200.f;
}

// End Sprinting
void ABeatEmUpCharacter::StopSprint()
{
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed-=200.f;
}

// Rewind: Get back to previous location a deal damage to enemies and push back
void ABeatEmUpCharacter::Rewind()
{
	if(bCanRewind)
	{
		if(RewindEffectClass)
		{
			
			 UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),RewindEffectClass,SpawnTeleport->GetActorLocation(), SpawnTeleport->GetActorRotation());
		}
		FVector TeleportLocation = SpawnTeleport->GetActorLocation();
		SetActorLocation(TeleportLocation);
		SpawnTeleport->Destroy();
		bAlreadySetUp = false;

		TArray<FHitResult>Hits;
		TArray<AActor*>HitActors;
		FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(SweepSize);
		bool bSweep = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_WorldDynamic, ExplosionSphere);
		if(bSweep)
		{
			for(FHitResult Hit : Hits)
			{
				if(!HitActors.Contains(Hit.GetActor()))
				{
					HitActors.Add(Hit.GetActor());
					UMeshComponent* HitMesh;
					AEnemy* HitEnemy = Cast<AEnemy>(Hit.GetActor());
					if(HitEnemy)
					{
						HitMesh = Cast<UMeshComponent>(HitEnemy->GetMesh());
						HitEnemy->Ragdoll();
					}
					else
					{
						HitMesh = Cast<UMeshComponent>(Hit.GetActor()->GetRootComponent());
					}
					if(HitMesh){
						HitMesh->AddRadialImpulse(GetActorLocation(), SweepSize, RewindDamage, RIF_Linear, true);
  	
					}
				}
			}
		}
	}
}


void ABeatEmUpCharacter::PauseGame()
{
	if(!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UPauseMenu* PauseMenu = Cast<UPauseMenu>(CreateWidget(GetGameInstance(),PauseMenuClass));
		if(PauseMenu)
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			PauseMenu->AddToViewport();
		}
	}

}

void ABeatEmUpCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABeatEmUpCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}