// Fill out your copyright notice in the Description page of Project Settings.


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
	Camera->SetupAttachment(SpringArm);
	
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

	// 방향 부여 
	FVector diru = FVector::ForwardVector * u;
	FVector dirr = FVector::RightVector * r;
	FVector dir = diru + dirr;
	dir.Normalize();
	FVector p0 = GetActorLocation();
	FVector dt = dir * speed * DeltaTime;
	SetActorLocation(p0 + dt);

}

// Called to bind functionality to input
void AHJMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 플레이어컴포넌트 움직임 부여 
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AHJMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AHJMainCharacter::LeftRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHJMainCharacter::InputJump);
}

void AHJMainCharacter::UpDown(float value)
{
	u = value;
}

void AHJMainCharacter::LeftRight(float value)
{
	r = value;
}

void AHJMainCharacter::InputJump()
{
	Jump();
}

