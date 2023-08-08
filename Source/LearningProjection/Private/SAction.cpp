// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"


void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"),*GetNameSafe(this));
	
	USActionComponent* Comp = GetOwningComponent();

	/*
		AddTag只是添加单个Tag进TagContainer里
		AppendTags则可以添加整个TagContainer到另一个TagContainer中
	*/
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
}

UWorld* USAction::GetWorld() const
{
	/*
		T* NewObject(UObject* Outer, const UClass* Class, ......)
		在SActionCompoent->AddAction中的NewObject<USAction>(this, ActionClass)参数里的this就是此时的Outer
		Outer is set when creating action via NewObject<1>
	*/
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());

	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	/*在SActionCompoent->AddAction中的NewObject<USAction>(this, ActionClass)保证了Outer一定不为空*/
	return Cast<USActionComponent>(GetOuter());
}