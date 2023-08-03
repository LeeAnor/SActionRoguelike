// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.17f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (ensure(Character))
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackHandVFX, Character->GetMesh()->GetSocketLocation(HandSocketName), Character->GetActorRotation());

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		/*GameModeBase中的GetTimerManager()是在ACharacter中的类成员函数，而USAction是直接继承至UObject的，所以此时使用GetWorld()*/
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);

	}

	

}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("Muzzle_01");	//从骨骼中获取对应Socket的位置
		FVector TraceStartLocation = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEndLocation = TraceStartLocation + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FCollisionObjectQueryParams ObjParams;	//声明碰撞参数
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;	//设置出手时忽略与手部的碰撞
		Params.AddIgnoredActor(InstigatorCharacter);

		FActorSpawnParameters SpawnParameters;	//声明SpawnActor()函数的生成参数
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//设置SpawnActor()函数的生成参数
		SpawnParameters.Instigator = InstigatorCharacter;

		FHitResult Hit;
		//GetWorld()->SweepSingleByObjectType(Hit, TraceStartLocation, TraceEndLocation, FQuat::Identity, ObjParams, Shape, Params);
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStartLocation, TraceEndLocation, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEndLocation = Hit.ImpactPoint;
		}

		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEndLocation);
		//FRotator SpawnRotation = FRotationMatrix::MakeFromX(TraceEndLocation - HandLocation).Rotator();  

		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParameters);
		//DrawDebugLine(GetWorld(), HandLocation, Hit.ImpactPoint, FColor::Green, false, 2.f, 0, 2.f);
	}

	StopAction(InstigatorCharacter);
}