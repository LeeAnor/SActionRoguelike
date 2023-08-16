// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributesComponent.h"
#include "SPlayerState.h"


ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	CreditCost = 50;
}

void ASPowerup_HealthPotion::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("SubClassOfPotion override the OnActorOverlap"));
	//return;
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigitorPawn)
{
	if (!ensure(InstigitorPawn))
	{
		return;
	}

	USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(InstigitorPawn);
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigitorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetMaxHealth()))
			{
				HideAndCooldownHealthPotion();
			}
		}
	}
}
