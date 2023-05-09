// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributesComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	
	DamageAmount = -20.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor!=GetInstigator())
	{
		USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(OtherActor);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), DamageAmount);
		}
		Explode();
	}
}

void ASMagicProjectile::Explode_Implementation()
{
	Super::Explode_Implementation();
}


