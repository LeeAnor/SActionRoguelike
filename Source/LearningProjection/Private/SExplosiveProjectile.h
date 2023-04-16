// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SExplosiveProjectile.generated.h"

class URadialForceComponent;
UCLASS()
class ASExplosiveProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeComp;

	TObjectPtr<URadialForceComponent> ForceComp;

protected:
	virtual void Explode_Implementation() override;

public:
	ASExplosiveProjectile();
};
