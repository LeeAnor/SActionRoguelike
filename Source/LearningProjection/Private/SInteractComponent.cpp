// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"
#include "SGamePlayInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su. Interact ionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);


void USInteractComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	//FHitResult Hit;
	
	//bool IsHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	float Radius = 30.f;
	TArray<FHitResult> HitTarry;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	bool IsHit = GetWorld()->SweepMultiByObjectType(HitTarry, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
	FColor LineColor = IsHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : HitTarry)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);

				if(bDebugDraw) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
				
				break;
			}
		}
	}
	
	if(bDebugDraw) DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0, 2.f);
}
