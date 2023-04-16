// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributesComponent.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASHealthPotion::OnActorOverlap);
}

void ASHealthPotion::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (PlayerPawn)
		{
			SetActorEnableCollision(false);
			MeshComp->SetVisibility(false);

			USAttributesComponent* PlayerAttributeComp = Cast<USAttributesComponent>(PlayerPawn->GetComponentByClass(USAttributesComponent::StaticClass()));
			if (PlayerAttributeComp)
			{
				PlayerAttributeComp->ApplyHealthChange(50.0f);
				GetWorldTimerManager().SetTimer(TimerHandle_HealthPotionCooldown, this, &ASHealthPotion::ReSpawnHealthPotion, 10.0f);
			}
		}
	}
}

void ASHealthPotion::ReSpawnHealthPotion()
{
	SetActorEnableCollision(true);
	MeshComp->SetVisibility(true);
}


