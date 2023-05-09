// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveProjectile.h"
#include "AI/SAICharacter.h"
#include "Components/SphereComponent.h"
#include "SAttributesComponent.h"
#include "LearningProjection/SCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASExplosiveProjectile::OnActorOverlap);
	DamageAmount = -40.0f;
}

void ASExplosiveProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(OtherActor);
		if (AttributeComp)
		{
			ApplyRangedDamage();
		}
		Explode();
	}
}

void ASExplosiveProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplyRangedDamage();
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ASExplosiveProjectile::ApplyRangedDamage()
{
	
	TArray<FHitResult> OutHits;
	// start and end locations. The sphere will create the radial sweep.
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation();
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(250.0f);
	// draw collision sphere
	//DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Cyan, true, 2.0f);

	FCollisionObjectQueryParams ObjParams;	//声明碰撞参数
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	/*直接使用OutHits.GetActor()返回的Actor类包含不同的物理碰撞骨骼(如脊椎、头)*/
	/*加入TA_ReceivedDamagePawn确定可被造成伤害的Pawn, 防止同一个Pawn应用多次伤害*/
	FCollisionQueryParams Param;
	Param.AddIgnoredComponent(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh());
	TArray<AActor*> TA_AICharacter;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASAICharacter::StaticClass(), TA_AICharacter);
	for (auto& AIActor : TA_AICharacter)
	{
		Param.AddIgnoredComponent(Cast<ASAICharacter>(AIActor)->GetMesh());
	}

	if (GetWorld()->SweepMultiByObjectType(OutHits, Start, End, FQuat::Identity, ObjParams, MyColSphere,Param))
	{
		for (auto& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				USAttributesComponent* Attributes = USAttributesComponent::GetAttributes(HitActor);
				if (Attributes)
				{
					Attributes->ApplyHealthChange(GetInstigator(), DamageAmount);
				}
			}
		}

	}
}
void ASExplosiveProjectile::Explode_Implementation()
{
	ForceComp->FireImpulse();
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShakeComp, GetActorLocation(), 0.0f, 1000.0f);	//设置摄像机抖动

	Super::Explode_Implementation();
}


