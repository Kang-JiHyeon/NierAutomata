// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeamSkill.h"
#include "JHEnemy.h"
#include "JHLaserBeam.h"

// Sets default values for this component's properties
UJHLaserBeamSkill::UJHLaserBeamSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UJHLaserBeamSkill::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UJHLaserBeamSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAttack && bRotate)
	{
		if (MyEnemy != nullptr)
		{
			MyEnemy->RotateSpinBottom();
		}
	}
}

void UJHLaserBeamSkill::OnInitialize()
{
	Super::OnInitialize();

}

void UJHLaserBeamSkill::OnAttack()
{
	Super::OnAttack();

	if (SkillInfoByLevel.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserBeamSkill : SKillInfoByLevel Null!!"));
		return;
	}

	if (!bCreatedBeam)
	{
		// LaserBeam 생성
		CreateLaserBeams();

		// 대기 시간이 지나면 공격 상태로 전환한다.
		GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &UJHLaserBeamSkill::SetActiveAttack, SkillInfoByLevel[CurrSkillLevel].IdleTime, false);

		bCreatedBeam = true;
	}
}

void UJHLaserBeamSkill::OnEnd()
{
	Super::OnEnd();

	bRotate = false;
	bCreatedBeam = false;
	DestroyLaserBeams();
}

/// <summary>
/// LaserBeam 생성
/// </summary>
void UJHLaserBeamSkill::CreateLaserBeams()
{
	int32 MaxCount = SkillInfoByLevel[CurrSkillLevel].MaxCount;

	for (int32 i = 0; i < MaxCount; i++)
	{
		FRotator Rotation = FRotator(0, i * (360 / MaxCount), 0);

		// LaserBeam 생성
		AJHLaserBeam* LaserBeam = GetWorld()->SpawnActor<AJHLaserBeam>(SkillFactory, GetComponentLocation(), GetComponentRotation());
		LaserBeam->SetActorRotation(LaserBeam->GetActorRotation() + Rotation);
		LaserBeam->SetActorLocation(LaserBeam->GetActorLocation() + LaserBeam->GetActorForwardVector() * Radius);

		USceneComponent* Root = LaserBeam->GetRootComponent();
		Root->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		LaserBeam->SetIdleTime(SkillInfoByLevel[CurrSkillLevel].IdleTime);
		LaserBeams.Add(LaserBeam);
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateLaserBeams!!"));
}

/// <summary>
/// LaserBeam 제거
/// </summary>
void UJHLaserBeamSkill::DestroyLaserBeams()
{
	if (LaserBeams.IsEmpty())
		return;

	for (auto LaserBeam : LaserBeams)
	{
		LaserBeam->Destroy();
	}
	LaserBeams.Empty();

	UE_LOG(LogTemp, Warning, TEXT("DestroyLaserBeams!!"));
}

/// <summary>
/// 공격 활성화
/// </summary>
void UJHLaserBeamSkill::SetActiveAttack()
{
	// 회전 여부 활성화
	bRotate = true;

	// 회전 속도 변경
	MyEnemy->SetRotSpeed(SkillInfoByLevel[CurrSkillLevel].RotSpeed);

	// 공격 시간이 지난 후 공격을 종료
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UJHLaserBeamSkill::OnEnd, SkillInfoByLevel[CurrSkillLevel].AttackTime);
}

