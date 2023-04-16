// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPcgMazeGenerate.generated.h"

UCLASS()
class ASPcgMazeGenerate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPcgMazeGenerate();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Params")
	int rows;

	UPROPERTY(EditDefaultsOnly, Category = "Params")
	int cols;

	UPROPERTY(EditDefaultsOnly, Category = "Params")
	int RoomExtent;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshFloor;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TSubclassOf<AActor> MeshWall;

	TArray<TArray<int>> MazeTypes;
	TArray<TArray<TArray<bool>>> MazeDoors;

	void GenerateMaze();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
