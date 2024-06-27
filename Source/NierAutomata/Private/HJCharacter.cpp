// Fill out your copyright notice in the Description page of Project Settings.


#include "HJCharacter.h"
#include "HJWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHJCharacter::AHJCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ī�޶� & �������� ������Ʈ ���� 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));

	// �������Ͽ� ī�޶� ��ġ 
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 800;
	SpringArm->bUsePawnControlRotation = true;
	Camera->SetupAttachment(SpringArm);
	// ���ɷ�Ż �޽� ��ü �ο� 
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ControlRig/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// ���� 
	JumpMaxCount = 2;
	GetCharacterMovement()->JumpZVelocity = 800.0f;
}

// Called when the game starts or when spawned
void AHJCharacter::BeginPlay()
{
	Super::BeginPlay();
	// ��� ���� (�⺻�ӵ�) 
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	// ���� �߰� 
	FName WeaponSocket(TEXT("middle_01_r"));
	auto CurWeapon = GetWorld()->SpawnActor<AHJWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
}

// Called every frame
void AHJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// �յ��¿� �̵� 
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AHJCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AHJCharacter::InputVertical);
	// ��� ���� 
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AHJCharacter::StartDash);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, this, &AHJCharacter::EndDash);
	// ���� ���� 
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHJCharacter::InputJump);
}

// �յ��¿� �̵�
void AHJCharacter::InputHorizontal(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}
void AHJCharacter::InputVertical(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

// ��� ����
void AHJCharacter::StartDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
}
void AHJCharacter::EndDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
}

// ����
void AHJCharacter::InputJump()
{
	Jump();
}

// ī�޶� ȸ�� 
void AHJCharacter::InputTurn(float value)
{
	AddControllerYawInput(value);
}
void AHJCharacter::InputLookup(float value)
{
	AddControllerPitchInput(value);
}

