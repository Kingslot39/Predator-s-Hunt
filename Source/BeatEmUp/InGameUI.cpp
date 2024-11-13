// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "BeatEmUpCharacter.h"



void UInGameUI::UpdateValues()
{
	HealthBar->SetPercent((float)Player->CurrentHealth/Player->MaxHealth);
	ComboCount->SetText(FText::FromString(FString::FromInt(Player->ComboCounter)));
	CurrentHealthText->SetText(FText::FromString(FString::FromInt(Player->CurrentHealth)));
	MaxHealthText->SetText(FText::FromString(FString::SanitizeFloat(Player->MaxHealth)));
	EXPBar->SetPercent((float)Player->CurrentEXP / Player->EXPToLevel);
	float PunchTimePercentage = 0;
	if(GetWorld()->GetTimerManager().TimerExists(Player->PunchTimerHandle))
	{
		PunchTimePercentage = GetWorld()->GetTimerManager().GetTimerRemaining(Player->PunchTimerHandle) / Player->PunchCoolDown;
	}
	PunchCooldown->SetPercent(1 - PunchTimePercentage);
	
}




