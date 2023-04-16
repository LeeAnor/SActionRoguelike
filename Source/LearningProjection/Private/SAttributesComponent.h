// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMaxHealthChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewMaxHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnExpChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewXP, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Exp;

public:	

	USAttributesComponent();

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category="AttributesChanged")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "AttributesChanged")
	FOnMaxHealthChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "AttributesChanged")
	FOnExpChanged OnExpChanged;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const {return MaxHealth; };

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return Health > 0.0f; };


	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyMaxHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyExpChange(float Delta);

};
