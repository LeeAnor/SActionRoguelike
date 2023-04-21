// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	void PrimaryInteract();
		
};
