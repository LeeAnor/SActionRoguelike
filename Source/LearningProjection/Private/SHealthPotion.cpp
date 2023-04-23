// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributesComponent.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	CooldownTimer = 10.0f;

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASHealthPotion::OnActorOverlap);
}

void ASHealthPotion::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (PlayerPawn)
		{
			USAttributesComponent* PlayerAttributeComp = Cast<USAttributesComponent>(PlayerPawn->GetComponentByClass(USAttributesComponent::StaticClass()));
			if (PlayerAttributeComp)
			{
				if (PlayerAttributeComp->ApplyHealthChange(this, 50.0f)) 
				{
					HideAndCooldownHealthPotion();
				}
			}
		}
	}
}

void ASHealthPotion::HideAndCooldownHealthPotion()
{
	SetActorEnableCollision(false);
	RootComponent->SetVisibility(false);
	GetWorldTimerManager().SetTimer(TimerHandle_HealthPotionCooldown, this, &ASHealthPotion::ReSpawnHealthPotion, CooldownTimer);
}

void ASHealthPotion::ReSpawnHealthPotion()
{
	SetActorEnableCollision(true);
	RootComponent->SetVisibility(true);
}


