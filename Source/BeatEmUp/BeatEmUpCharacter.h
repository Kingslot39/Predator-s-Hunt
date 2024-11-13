// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InGameUI.h"
#include "NiagaraFunctionLibrary.h"
#include "Landmark.h"
#include "PauseMenu.h"
#include "Shuriken.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BeatEmUpCharacter.generated.h"

class AEnemy;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABeatEmUpCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Punched;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Used;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RewindAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;


public:
	ABeatEmUpCharacter();
	UPROPERTY(EditAnyWhere)
	float MaxHealth = 100;
	UPROPERTY(EditAnyWhere)
	int CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere)
	UInputAction* PunchAction;
	UPROPERTY(EditAnywhere)
	float PunchDistance = 250;
	UPROPERTY(EditAnywhere)
	float PunchForce = 50000;
	UPROPERTY(EditAnywhere)
	float PunchDamage = 50;
	UPROPERTY(EditAnywhere)
	float PunchCoolDown = 1;
	UPROPERTY(EditAnywhere)
	float RewindDamage = 75;
	UPROPERTY(EditAnywhere)
	int EnemyKills;
    UPROPERTY()
	AActor* SpawnTeleport;

	UPROPERTY(EditAnywhere)
	float MaxATK = 50;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ALandmark>LandmarkClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ALandmark* Landmark;

	UPROPERTY(EditAnywhere)
	float SweepSize = 500;

   
	UPROPERTY(EditAnywhere)
     UInputAction* UseAction;
    UPROPERTY(EditAnywhere)
     float UseDistance = 1000;
    UFUNCTION()
     void Use();

	UPROPERTY(EditAnywhere)
	AActor* TargetActor;
   UFUNCTION()
	void Shooting();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShuriken>ShurikenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameUI> InGameUIClass;
    UPROPERTY()
	UInGameUI* InGameUI;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere)
	int ComboCounter;
	

	UPROPERTY(EditAnywhere)
	int CurrentEXP =0;
	UPROPERTY(EditAnywhere)
	int EXPToLevel = 50;
	UPROPERTY(EditAnywhere)
	float IncreaseMultiplier = 1.5;

	void AddEXP(int EXPToAdd);
 
	
	bool bPunchReady = true;
	FTimerHandle PunchTimerHandle;

	UFUNCTION()
	void Punch();
	UFUNCTION()
	 void ResetPunch();
	void DealDamage(float Damage);
	
	UFUNCTION()
	void Rewind();

	UFUNCTION()
	void SetUp();

	void StartSprint();
	void StopSprint();

	
    bool IsSprinting;
	bool bCanRewind = false;
	bool bAlreadySetUp = false;

	UPROPERTY(EditAnywhere)
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere)
	  TSubclassOf<UPauseMenu>PauseMenuClass;
	UFUNCTION()
	void PauseGame();

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PunchEffectClass;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* RewindEffectClass;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

