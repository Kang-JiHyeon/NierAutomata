// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeamSkill.h"
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

	for (int32 i = 0; i < MaxCount; i++)
	{
		// 위치
		//FVector Location = GetPositionOnCircle(Radius, i * (360 / MaxCount), GetComponentLocation());
		//// 회전
		//FVector TargetDirection = Location - GetComponentLocation();
		FRotator Rotation = FRotator(0, i * (360 / MaxCount), 0);

		// LaserBeam 생성
		AJHLaserBeam* LaserBeam = GetWorld()->SpawnActor<AJHLaserBeam>(SkillFactory);

		USceneComponent* Root = LaserBeam->GetRootComponent();
		Root->SetupAttachment(this);
		Root->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		LaserBeam->SetActorRelativeRotation(Rotation);
		LaserBeam->SetActorRelativeLocation(LaserBeam->GetActorForwardVector() * Radius);

		LaserBeams.Add(LaserBeam);
	}

	ToggleEnableActor();

}


// Called every frame
void UJHLaserBeamSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UJHLaserBeamSkill::OnInitialize()
{
	if (LaserBeams.Num() <= 0) return;

	bEnable = false;
	ToggleEnableActor();
}

void UJHLaserBeamSkill::OnAttack()
{
	if (!bEnable)
	{
		bEnable = true;
		ToggleEnableActor();
	}
	else
	{

	}
}

void UJHLaserBeamSkill::ToggleEnableActor()
{
	for (auto LaserBeam : LaserBeams)
	{
		// 액터의 노출
		LaserBeam->SetActorHiddenInGame(!bEnable);

		// 액터의 충돌
		LaserBeam->SetActorEnableCollision(bEnable);

		// 액터의 틱
		LaserBeam->SetActorTickEnabled(bEnable);

		if(!bEnable)
			LaserBeam->SetCurrIdleTime(0);
	}
}

FVector UJHLaserBeamSkill::GetPositionOnCircle(float TargetRadius, float TargetDegree, FVector CenterPos)
{
	float Radian = FMath::DegreesToRadians(TargetDegree);
	float X = CenterPos.X + TargetRadius * FMath::Cos(Radian);
	float Y = CenterPos.Y + TargetRadius * FMath::Sin(Radian);

	return FVector(X, Y, CenterPos.Z);
}
