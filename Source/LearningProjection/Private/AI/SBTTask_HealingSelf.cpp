// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealingSelf.h"
#include "AIController.h"
#include "SAttributesComponent.h"

EBTNodeResult::Type USBTTask_HealingSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributesComponent* Attributes = USAttributesComponent::GetAttributes(MyPawn);
	if (Attributes)
	{
		Attributes->ApplyHealthChange(MyPawn, Attributes->GetMaxHealth());
	}
	
	return EBTNodeResult::Succeeded;
}
