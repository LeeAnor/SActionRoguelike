// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SPrimaryProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ASPrimaryProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

};
