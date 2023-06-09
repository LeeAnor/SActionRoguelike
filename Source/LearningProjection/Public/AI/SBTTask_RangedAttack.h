// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGPROJECTION_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bScatter;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ScatterAngel;
};
