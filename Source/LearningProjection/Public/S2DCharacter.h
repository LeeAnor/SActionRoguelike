// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "S2DCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractComponent;
class USAttributesComponent;
class UAnimMontage;

UCLASS()
class LEARNINGPROJECTION_API AS2DCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmCmp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraCmp;

	UPROPERTY(VisibleAnywhere, Category = "components")
	USInteractComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "components")
	USAttributesComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	void PrimaryAttack();
	void PrimaryInteract();


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
