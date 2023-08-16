// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "Components/AudioComponent.h"
#include "GameplayTagContainer.h"
#include "SMagicProjectile.generated.h"


class UAudioComponent;
class USActionEffect;

UCLASS()
class ASMagicProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "DamageAmount")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "DamageAmount")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "DamageAmount")
	TSubclassOf<USActionEffect>  BurningActionClass;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
