// Fill out your copyright notice in the Description page of Project Settings.


#include "RageTask.h"

#include "EnemyBTController.h"

EBTNodeResult::Type URageTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTCompt = &OwnerComp;
	if(!BTCompt)
	{
		return EBTNodeResult::Failed;
	}
	AEnemyBTController* BTController = Cast<AEnemyBTController>(BTCompt->GetOwner());
	if(!BTController)
	{
		return EBTNodeResult::Failed;
	}
	BTController->RageStatusUpdate();
	return EBTNodeResult::Succeeded;
}
