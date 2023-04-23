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

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "components")
	USAttributesComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category="Material")
	FName TimeToHitParamName;

	void SetTargetActor(AActor* NewTargetActor);

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);
};
