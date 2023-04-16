// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveProjectile::ASExplosiveProjectile()
{
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(RootComponent);

	// Leaving this on applies small constant force via component 'tick' (Optional)
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 250.0f;
	ForceComp->ImpulseStrength = 600.0f; // Alternative: 200000.0 if bImpulseVelChange = false
	// Optional, ignores 'Mass' of other objects (if false, the impulse strength will be much higher to push most objects depending on Mass)
	ForceComp->bImpulseVelChange = true;

	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}
void ASExplosiveProjectile::Explode_Implementation()
{
	ForceComp->FireImpulse();
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShakeComp, GetActorLocation(), 0.0f, 1000.0f);	//…Ë÷√…„œÒª˙∂∂∂Ø

	Super::Explode_Implementation();
}


