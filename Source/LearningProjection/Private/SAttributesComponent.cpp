// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"




// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	MaxHealth = 100.0f;
	Health = 100.0f;
	Exp = 0.0f;
}

bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
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
USAttributesComponent* USAttributesComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributesComponent>(FromActor->GetComponentByClass(USAttributesComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributesComponent::IsActorAlive(AActor* Actor)
{
	USAttributesComponent* AttributesComp = GetAttributes(Actor);
	if (AttributesComp)
	{
		return AttributesComp->IsAlive();
	}
	return false;
}

/*¿ØÖÆÌ¨Execº¯Êý*/
bool USAttributesComponent::KillSelf(AActor* intigator)
{
	return ApplyHealthChange(intigator, -GetMaxHealth());
}