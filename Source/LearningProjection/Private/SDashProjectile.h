// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ASDashProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="Teleport")
	float TeleportDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DetonateDelay;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	ASDashProjectile();
};
