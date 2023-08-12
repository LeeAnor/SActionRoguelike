// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"


bool USAction::CanStart_Implementation(AActor* Instigator)
{
	/*
	* 只有当CanStart()返回true时，才会执行StartAction
	* 如果第一次运行StartAction使bIsRunning=true后，后续的CanStart()返回false，即后续的StartAction不会执行
	*/
	if (bIsRunning)
	{
		return false;
	}

	USActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"),*GetNameSafe(this));
	
	USActionComponent* Comp = GetOwningComponent();

	/*
	* AddTag只是添加单个Tag进TagContainer里
	* AppendTags则可以添加整个TagContainer到另一个TagContainer中
	*/
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));

	/* 
	* 当按下鼠标左键后调用StartAction，使用定时器延迟0.17s的时间用于播放挥动手臂动画，0.17s之后再生成Projectile并调用StopAction
	* 而每一个定时器是异步执行；
	* 在第一个StartAction与第一个StopAction的间隔时间中，bIsRunning的值一直为真；
	* 而在之后调用的若干个StartAction，由于异步执行可能会出现：第一个StopAction使bIsRunning=false,而紧接着会不执行第二个Startaction使bIsRunning=true
	* 却继续执行第二个StopAction，此时因为异步而连续执行两次StopAction
	* 第二次的StopAction中bIsRunning=false，导致断言ensureAlways(bIsRunning)报错
	* 
	* 在CanStart()中加入 if(bIsRunning) return false; 后避免出现以上问题，保证bIsRunning始终为true
	*/
	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
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

bool USAction::IsRunning() const
{
	return bIsRunning;
}