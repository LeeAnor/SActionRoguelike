// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyAIController = OwnerComp.GetAIOwner();
			if (ensure(MyAIController))
			{
				APawn* MyPawn = MyAIController->GetPawn();
				if (ensure(MyPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), MyPawn->GetActorLocation());

					bool bWithinRange = DistanceTo <= CheckRadius;

					bool bCanSighted = false;
					if (bWithinRange) 
					{
						bCanSighted = MyAIController->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bCanSighted));
				}
			}
		}
	}
}
