// Fill out your copyright notice in the Description page of Project Settings.


#include "HJCharacter.h"
#include "HJWeapon.h"
#include "HJBullet2.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AHJCharacter::AHJCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ī�޶� & �������� ������Ʈ
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRwINGARM"));

	// ī�޶� ��ġ 
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 800;
	SpringArm->bUsePawnControlRotation = true;

	// ī�޶� ȸ��
	bUseControllerRotationYaw = true;

	// ĳ���� ȸ�� 
	/*GetCharacterMovement()->bUseControllerDesiredRotation = true;*/
	
	// ���ɷ�Ż �޽�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ControlRig/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

}

// Called when the game starts or when spawned
void AHJCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� 
	JumpMaxCount = 2;
	// ���� ���� (�⺻�ӵ�) 
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	// ���� ���� (�⺻�߷�)
	GetCharacterMovement()->JumpZVelocity = 1200.0f;
	GetCharacterMovement()->GravityScale = 2.5;

	// ����
	FName WeaponSocket(TEXT("sky_attack_socket"));

	CurrentWeapon = GetWorld()->SpawnActor<AHJWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
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
	// �յ��¿�
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AHJCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AHJCharacter::InputVertical);

	// ī�޶� ȸ�� 
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHJCharacter::InputTurn);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &AHJCharacter::InputLookup);

	// ����
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AHJCharacter::StartDash);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, this, &AHJCharacter::EndDash);

	// �뽬
	PlayerInputComponent->BindAction(TEXT("Dash2"), IE_DoubleClick, this, &AHJCharacter::InputDash);

	// ���� ���� 
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHJCharacter::InputJump);
	// ���� �߷�
	PlayerInputComponent->BindAction(TEXT("Land"), IE_Pressed, this, &AHJCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Land"), IE_Released, this, &AHJCharacter::EndJump);

	// ���� 
	PlayerInputComponent->BindAction(TEXT("Attack3"), IE_Pressed, this, &AHJCharacter::StartAttack);
	PlayerInputComponent->BindAction(TEXT("Attack3"), IE_Released, this, &AHJCharacter::EndAttack);

	// ������ ���� 
	PlayerInputComponent->BindAction(TEXT("Attack2"), IE_Pressed, this, &AHJCharacter::FireLaser);



}

// �յ��¿�
void AHJCharacter::InputHorizontal(float value)
{
	if (value != 0.0f)
	{
		DashDirection.X = value;
	}
}
void AHJCharacter::InputVertical(float value)
{
	if (value != 0.0f)
	{
		DashDirection.Y = value;
	}
}

// ����
void AHJCharacter::StartDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 1600.0f;
}
void AHJCharacter::EndDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
}
// �뽬
void AHJCharacter::InputDash()
{
	FVector InputDirection = FVector::ZeroVector;

	FRotator ControlRotation = Controller->GetControlRotation();
	FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	InputDirection = (ForwardDirection * DashDirection.X + RightDirection * DashDirection.Y).GetSafeNormal();

	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	LaunchCharacter(InputDirection * DashDistance, true, true);
}

// ����
void AHJCharacter::InputJump()
{
	Jump();
}
// ���� �߷� 
void AHJCharacter::StartJump()
{
	GetCharacterMovement()->GravityScale = 0.5;
}

void AHJCharacter::EndJump()
{
	GetCharacterMovement()->GravityScale = 2.8;
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

// ���� ���� 
void AHJCharacter::EndAttack()
{
	
}

void AHJCharacter::FireLaser()
{
	UE_LOG(LogTemp, Warning, TEXT("FireLaser function called"));
	// ���� �������� ���� �������� �ָ� ���� ������ 
	FVector StartPoint = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndPoint = StartPoint + (ForwardVector * 1000.0f);

	if (LaserBeam)
	{
		LaserBeam->Destroy();
	}

	LaserBeam = GetWorld()->SpawnActor<AHJBullet2>(AHJBullet2::StaticClass(), StartPoint, FRotator::ZeroRotator);
	if (LaserBeam)
	{
		LaserBeam->FireLaser(StartPoint, EndPoint);
	}

}

void AHJCharacter::StartAttack()
{
}

// ���� �̵� 
void AHJCharacter::StartWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	FName WeaponSocket2(TEXT("hand_lSocket"));

	/*auto CurrentWeapon = GetWorld()->SpawnActor<AHJWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);*/

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);
	}
}

void AHJCharacter::BackWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	FName WeaponSocket(TEXT("sky_attack_socket"));

	/*auto CurrentWeapon = GetWorld()->SpawnActor<AHJWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);*/

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
}
