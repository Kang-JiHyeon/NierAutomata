// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/CharacterMovementComponent.h"
#include "HJAnimInstance2.h"
#include "HJMainCharacter.h"
#include "HJPet.h"
#include "GameFramework/Character.h"

// Sets default values
AHJMainCharacter::AHJMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERAARM"));

	// 카메라 거치대에 카메라 부착 
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation = true;
	Camera->SetupAttachment(SpringArm);

	// 카메라 회전 부여 
	/*Camera->bUsePawnControlRotation = false;*/
	/*bUseControllerRotationYaw = true;*/
	SpringArm->bUsePawnControlRotation = true;

	// 캐릭터에 스케레탈 메쉬 부여 
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// Pet 엑터가 부착할 컴포넌트 생성 
	petPos = CreateDefaultSubobject<USceneComponent>(TEXT("PET"));
	petPos->SetupAttachment(RootComponent);
	petPos->SetRelativeLocation(FVector(-100, 0, 200));
	
	// Pet 공장의 실체 
	ConstructorHelpers::FClassFinder<AHJPet> 
		tempPet(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_HJPet.BP_HJPet_C'"));
	if (tempPet.Succeeded())
	{
		petFactory = tempPet.Class;
	}

	// 점프 효과 부여 
	JumpMaxCount = 2;
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	// 이동속도 조정 
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	// 애니메이션 부여 
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	ConstructorHelpers::FClassFinder<UAnimInstance>
		CharAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/HJAnimation.HJAnimation_C'"));
	if (CharAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CharAnim.Class);
	}

}

// Called when the game starts or when spawned
void AHJMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Pet 엑터가 생성되는 위치 지정 
	AHJPet* pet = GetWorld()->SpawnActor<AHJPet>(petFactory);
	pet->SetActorLocation(petPos->GetComponentLocation());
}

// Called every frame
void AHJMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHJMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 플레이어컴포넌트 움직임 부여 
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AHJMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AHJMainCharacter::LeftRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHJMainCharacter::InputJump);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AHJMainCharacter::InputDash);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AHJMainCharacter::InputAttack);
	// 카메라 컴포넌트 회전 움직임 부여 
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHJMainCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &AHJMainCharacter::Lookup);
}
// 움직임 구현 
void AHJMainCharacter::UpDown(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void AHJMainCharacter::LeftRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}
// 점프 구현 
void AHJMainCharacter::InputJump()
{
	Jump();
}
// 대쉬 구현 
void AHJMainCharacter::InputDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
}
// 공격 구현 
void AHJMainCharacter::InputAttack()
{
	auto AnimInstance = Cast<UHJAnimInstance2>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;

	AnimInstance->PlayAttackMontage();
}
// 카메라 회전 구현 
void AHJMainCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}
void AHJMainCharacter::Lookup(float value)
{
	AddControllerPitchInput(value);
}

