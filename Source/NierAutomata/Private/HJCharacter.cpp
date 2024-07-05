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

	// 카메라 & 스프링암 컴포넌트
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRwINGARM"));

	// 카메라 거치 
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 800;
	SpringArm->bUsePawnControlRotation = true;

	// 카메라 회전
	bUseControllerRotationYaw = true;

	// 캐릭터 회전 
	/*GetCharacterMovement()->bUseControllerDesiredRotation = true;*/
	
	// 스케레탈 메시
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

	// 점프 
	JumpMaxCount = 2;
	// 가속 구현 (기본속도) 
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	// 점프 구현 (기본중력)
	GetCharacterMovement()->JumpZVelocity = 1200.0f;
	GetCharacterMovement()->GravityScale = 2.5;

	// 무기
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
	// 앞뒤좌우
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AHJCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AHJCharacter::InputVertical);

	// 카메라 회전 
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHJCharacter::InputTurn);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &AHJCharacter::InputLookup);

	// 가속
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AHJCharacter::StartDash);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, this, &AHJCharacter::EndDash);

	// 대쉬
	PlayerInputComponent->BindAction(TEXT("Dash2"), IE_DoubleClick, this, &AHJCharacter::InputDash);

	// 점프 구현 
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHJCharacter::InputJump);
	// 점프 중력
	PlayerInputComponent->BindAction(TEXT("Land"), IE_Pressed, this, &AHJCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Land"), IE_Released, this, &AHJCharacter::EndJump);

	// 공격 
	PlayerInputComponent->BindAction(TEXT("Attack3"), IE_Pressed, this, &AHJCharacter::StartAttack);
	PlayerInputComponent->BindAction(TEXT("Attack3"), IE_Released, this, &AHJCharacter::EndAttack);

	// 레이저 공격 
	PlayerInputComponent->BindAction(TEXT("Attack2"), IE_Pressed, this, &AHJCharacter::FireLaser);



}

// 앞뒤좌우
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

// 가속
void AHJCharacter::StartDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 1600.0f;
}
void AHJCharacter::EndDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
}
// 대쉬
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

// 점프
void AHJCharacter::InputJump()
{
	Jump();
}
// 점프 중력 
void AHJCharacter::StartJump()
{
	GetCharacterMovement()->GravityScale = 0.5;
}

void AHJCharacter::EndJump()
{
	GetCharacterMovement()->GravityScale = 2.8;
}

// 카메라 회전 
void AHJCharacter::InputTurn(float value)
{
	AddControllerYawInput(value);
}
void AHJCharacter::InputLookup(float value)
{
	AddControllerPitchInput(value);
}

// 공격 구현 
void AHJCharacter::EndAttack()
{
	
}

void AHJCharacter::FireLaser()
{
	UE_LOG(LogTemp, Warning, TEXT("FireLaser function called"));
	// 시작 지점에서 엔드 지점으로 멀리 뻗어 나간다 
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

// 무기 이동 
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
