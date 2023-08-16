// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHealthPotion.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGPROJECTION_API ASPowerup_Credits : public ASHealthPotion
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	int32 CreditsAmount;

	ASPowerup_Credits();

	//UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
