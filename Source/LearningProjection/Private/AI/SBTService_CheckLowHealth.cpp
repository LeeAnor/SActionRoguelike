// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyAIController = OwnerComp.GetAIOwner();
	if (ensure(MyAIController))
	{
		APawn* MyPawn = MyAIController->GetPawn();
		if (ensure(MyPawn))
		{
			USAttributesComponent* AIAttributeComp = USAttributesComponent::GetAttributes(MyPawn);
			if (ensure(AIAttributeComp))
			{
				UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
				bool bIsLowHealth = (AIAttributeComp->GetHealth() / AIAttributeComp->GetMaxHealth()) <= LowHealthThreshold;
				BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
			}

		}
	}
}
