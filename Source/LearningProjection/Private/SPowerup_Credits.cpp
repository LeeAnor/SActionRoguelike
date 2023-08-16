// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Credits.h"
#include "SPlayerState.h"

ASPowerup_Credits::ASPowerup_Credits()
{
	CreditsAmount = 80;

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASPowerup_Credits::OnActorOverlap);
}

void ASPowerup_Credits::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (PlayerPawn)
		{
			if(ASPlayerState* PS = PlayerPawn->GetPlayerState<ASPlayerState>())
			{
				PS->AddCredits(CreditsAmount);
				HideAndCooldownHealthPotion();
			}
		}
	}
}
