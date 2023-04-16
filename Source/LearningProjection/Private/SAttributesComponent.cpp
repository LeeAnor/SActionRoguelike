// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	MaxHealth = 100.0f;
	Health = 100.0f;
	Exp = 0.0f;
}

bool USAttributesComponent::ApplyHealthChange(float Delta)
{
	
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

bool USAttributesComponent::ApplyMaxHealthChange(float Delta)
{
	MaxHealth += Delta;

	OnMaxHealthChanged.Broadcast(nullptr, this, MaxHealth, Delta);

	return true;
}

bool USAttributesComponent::ApplyExpChange(float Delta)
{
	Exp += Delta;

	OnExpChanged.Broadcast(nullptr, this, Exp, Delta);

	return true;
}


