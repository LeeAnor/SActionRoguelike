// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;
UCLASS()
class ASItemChest : public AActor , public ISGamePlayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigitorPawn) override;

public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UStaticMeshComponent* LidMesh;


};
