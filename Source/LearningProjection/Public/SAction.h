// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class LEARNINGPROJECTION_API USAction : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, category = "Action")
	void StopAction(AActor* Instigator);


	/*Action nickname to start / stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;
};
