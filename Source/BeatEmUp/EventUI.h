// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EventUI.generated.h"

class APredatorEvent;
/**
 * 
 */
UCLASS()
class BEATEMUP_API UEventUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EventSecondCooldown;


	void UpdateCooldown(int Timer);
	
	
	
};
