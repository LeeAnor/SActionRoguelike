// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.h"
#include "SGamePlayInterface.h"
#include "SPowerup_HealthPotion.generated.h"

UCLASS()
class LEARNINGPROJECTION_API ASPowerup_HealthPotion : public ASHealthPotion, public ISGamePlayInterface
{
	GENERATED_BODY()
	
	void Interact_Implementation(APawn* InstigitorPawn) override;

public:	
	// Sets default values for this actor's properties
	ASPowerup_HealthPotion();

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 CreditCost;

	//UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
