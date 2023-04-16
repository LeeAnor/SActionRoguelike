// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "LearningProjection/SCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributesComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComp;
	
	AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	//UE_LOG(LogTemp, Warning, TEXT("Dummy health is &f", AttributeComp->Health));
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());	//设置材质中的特定参数值，实现击中闪烁
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			USAttributesComponent* PlayerAttributeComp = Cast<USAttributesComponent>(PlayerPawn->GetComponentByClass(USAttributesComponent::StaticClass()));
			PlayerAttributeComp->ApplyExpChange(10.0f);
			//PlayerAttributeComp->ApplyMaxHealthChange(20.0f);
		}
	}
}


