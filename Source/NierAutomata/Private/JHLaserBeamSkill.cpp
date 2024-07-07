// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeamSkill.h"
#include "JHEnemy.h"
#include "JHLaserBeam.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	MyBoss = Cast<AJHEnemy>(GetOwner());
}


// Called every frame
void UJHLaserBeamSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UJHLaserBeamSkill::OnInitialize()
{
	bAttack = false;
	CurrIdleTime = 0;

	DestroyLaserBeams();
}

void UJHLaserBeamSkill::OnAttack()
{
	// 이미 공격 중이라면
	 if (bAttack)
	{
		// 일정 시간이 지나면 회전하고 싶다.
		if (MyBoss != nullptr)
		{
			if (CurrIdleTime <= IdleTime)
			{
				CurrIdleTime += GetWorld()->DeltaTimeSeconds;
			}
			else
			{
				MyBoss->RotateSpinBottom();
			}
		}
	}
	else
	{
		// LaserBeam 생성
		CreateLaserBeams();
		
		bAttack = true;
	}
}

void UJHLaserBeamSkill::CreateLaserBeams()
{
	for (int32 i = 0; i < MaxCount; i++)
	{
		FRotator Rotation = FRotator(0, i * (360 / MaxCount), 0);

		// LaserBeam 생성
		AJHLaserBeam* LaserBeam = GetWorld()->SpawnActor<AJHLaserBeam>(SkillFactory);

		USceneComponent* Root = LaserBeam->GetRootComponent();
		Root->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		LaserBeam->SetActorRelativeRotation(Rotation);
		LaserBeam->SetActorRelativeLocation(LaserBeam->GetActorForwardVector() * Radius);
		LaserBeam->SetIdleTime(IdleTime);
		LaserBeams.Add(LaserBeam);
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateLaserBeams!!"));
}

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

