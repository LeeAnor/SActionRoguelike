// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractComponent.h"
#include "SAttributesComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
AS2DCharacter::AS2DCharacter()
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

	//AttackAnimDelay = 0.17f;

}

// Called when the game starts or when spawned
void AS2DCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AS2DCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AS2DCharacter::OnHealthChanged);
}

// Called every frame
void AS2DCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS2DCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();	//让角色的前进方向为鼠标控制的方向
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void AS2DCharacter::MoveRight(float value)
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
void AS2DCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AS2DCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AS2DCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AS2DCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AS2DCharacter::PrimaryAttack);
	//PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &AS2DCharacter::BlackHoleAttack);
	//PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &AS2DCharacter::DashAttack);
	//PlayerInputComponent->BindAction("ExplodeAttack", IE_Pressed, this, &AS2DCharacter::ExplodeAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AS2DCharacter::PrimaryInteract);

}

void AS2DCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void AS2DCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{

}

void AS2DCharacter::PrimaryAttack()
{

}

void AS2DCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	/*
	if (Delta < 0.0f && NewHealth > 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());	//材质闪烁效果

	}
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		DisableInput(PC);
	}*/
}