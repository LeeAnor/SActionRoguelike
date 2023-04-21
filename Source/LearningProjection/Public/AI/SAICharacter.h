// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAttributesComponent.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributesComponent;
class UAnimMontage;

UCLASS()
class LEARNINGPROJECTION_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "components")
	USAttributesComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
