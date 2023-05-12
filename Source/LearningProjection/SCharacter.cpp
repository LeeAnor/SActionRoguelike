// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractComponent.h"
#include "SAttributesComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmCmp = CreateDefaultSubobject<USpringArmComponent>("SpringArmCmp");
	SpringArmCmp->SetupAttachment(RootComponent);
	SpringArmCmp->bUsePawnControlRotation = true;	//camera接受自Pawn身上的playerController的鼠标输入

	CameraCmp = CreateDefaultSubobject<UCameraComponent>("CameraCmp");
	CameraCmp->SetupAttachment(SpringArmCmp);

	InteractionComp = CreateDefaultSubobject<USInteractComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");

	//旋转角色朝向加速方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AttackAnimDelay = 0.17f;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();	//让角色的前进方向为鼠标控制的方向
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X is Forward(Red) ; Y is Right(Green) ; Z is Up(Blue) ;
	//让character转向camera的right方向，而不是转向character自身的right方向
	
	FVector rightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(rightVector, value);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &ASCharacter::DashAttack);
	PlayerInputComponent->BindAction("ExplodeAttack", IE_Pressed, this, &ASCharacter::ExplodeAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

}



void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");	//从骨骼中获取对应Socket的位置
		FVector TraceStartLocation = CameraCmp->GetComponentLocation();
		FVector TraceEndLocation = CameraCmp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionObjectQueryParams ObjParams;	//声明碰撞参数
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;	//设置出手时忽略与手部的碰撞
		Params.AddIgnoredActor(this);

		FActorSpawnParameters SpawnParameters;	//声明SpawnActor()函数的生成参数
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//设置SpawnActor()函数的生成参数
		SpawnParameters.Instigator = this;

		FHitResult Hit;
		//GetWorld()->SweepSingleByObjectType(Hit, TraceStartLocation, TraceEndLocation, FQuat::Identity, ObjParams, Shape, Params);
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStartLocation, TraceEndLocation, FQuat::Identity, ObjParams,Shape, Params))
		{
			TraceEndLocation = Hit.ImpactPoint;
		}

		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEndLocation);
		//FRotator SpawnRotation = FRotationMatrix::MakeFromX(TraceEndLocation - HandLocation).Rotator();  

		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParameters);
		//DrawDebugLine(GetWorld(), HandLocation, Hit.ImpactPoint, FColor::Green, false, 2.f, 0, 2.f);
	}
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f && NewHealth > 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());	//材质闪烁效果

	}
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		DisableInput(PC);
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	//Assignment3-2-2:尝试使用SpawnEmitterAttached代替SpawnEmitterAtLocation，Answers:P46
	//则暂时继续使用SpawnEmitterAtLocation实现手部发射弹丸时的粒子特效。
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackHandVFX, GetMesh()->GetSocketLocation("Muzzle_01"), GetActorRotation());

	SpawnProjectile(ProjectileClass);
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::DashAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_DashAttack, this, &ASCharacter::DashAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::ExplodeAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeAttack, this, &ASCharacter::ExplodeAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::ExplodeAttack_TimeElapsed()
{
	SpawnProjectile(ExplodeProjectileClass);
}

//Exec控制台函数
void ASCharacter::HealSelf(float Amount /* = 100.0*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}
