// Fill out your copyright notice in the Description page of Project Settings.


#include "EventUI.h"


void UEventUI::UpdateCooldown(int Timer)
{
	EventSecondCooldown->SetText(FText::FromString(FString::FromInt(Timer)));
}
