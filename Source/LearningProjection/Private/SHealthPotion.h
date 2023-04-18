// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class ASHealthPotion : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTimer;

	FTimerHandle TimerHandle_HealthPotionCooldown;

	void HideAndCooldownHealthPotion();
	void ReSpawnHealthPotion();

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Sets default values for this actor's properties
	ASHealthPotion();


};
