// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootTask.h"

#include "EnemyBTController.h"

EBTNodeResult::Type UShootTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	BTController->Shoot();
	return EBTNodeResult::Succeeded;
	
	
}
