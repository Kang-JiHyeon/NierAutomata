// Fill out your copyright notice in the Description page of Project Settings.


#include "JHSpiralMoveSkill.h"

// Sets default values for this component's properties
UJHSpiralMoveSkill::UJHSpiralMoveSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UJHSpiralMoveSkill::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();

	CenterPos = MyOwner->GetActorLocation();
}


// Called every frame
void UJHSpiralMoveSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���� �ð����� Ȯ���ϴ� �������� ���������� �̵��ϰ� �ʹ�.
	// ���� �ð��� ������ �ٽ� ����ϴ� �������� ������ �̵��ϰ� �ʹ�.
	// - ���� �ð�
	// - ����
	// - �̵� ���
	// - ���� ��ǥ
	// ����

	if (bIsAttack)
	{
		OnAttack();
	}
	else
	{
		OnInitialize();
	}

}

void UJHSpiralMoveSkill::OnInitialize()
{
	CurrTime = 0;
	Sign = 1;
	Radius = 0;
	DegreeAngle = 0;
	CenterPos = FVector(0, 0, 0);
}

void UJHSpiralMoveSkill::OnAttack()
{
	if (MyOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner�� Null�Դϴ�."));
		return;
	}

	//if (CurrTime < 0)
	//	Sign = 1;
	//else if (CurrTime > MoveTime)
	//	Sign = -1;
	if (CurrTime < 0)
		return;

	if (CurrTime > MoveTime)
		Sign = -1;


	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	CurrTime += Sign * DeltaTime;

	Radius += Sign * RadiusSpeed * DeltaTime;
	DegreeAngle += AngleSpeed * DeltaTime;

	if (DegreeAngle > 360)
		DegreeAngle = 0;

	float RadianAngle = FMath::DegreesToRadians(DegreeAngle);

	float X = CenterPos.X + FMath::Cos(RadianAngle) * Radius;
	float Y = CenterPos.Y + FMath::Sin(RadianAngle) * Radius;

	MyOwner->SetActorLocation(CenterPos + FVector(X, Y, 0));
}

void UJHSpiralMoveSkill::SetCenterPosition(FVector Position)
{
	CenterPos = Position;
}

