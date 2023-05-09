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

protected:
	TObjectPtr<URadialForceComponent> ForceComp;

	UPROPERTY(EditDefaultsOnly,Category="CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeComp;

	UPROPERTY(EditDefaultsOnly, Category = "DamageAmount")
	float DamageAmount;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void ApplyRangedDamage();

	virtual void Explode_Implementation() override;

public:
	ASExplosiveProjectile();
};
