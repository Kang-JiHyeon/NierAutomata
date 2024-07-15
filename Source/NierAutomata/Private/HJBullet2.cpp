// Fill out your copyright notice in the Description page of Project Settings.


#include "HJBullet2.h"
#include "Components/SplineComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AHJBullet2::AHJBullet2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*LaserBeamPSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserBeamPSC"));
	RootComponent = LaserBeamPSC;*/
	/*Laser = CreateDefaultSubobject<USplineComponent>(TEXT("LASER"));
	SetRootComponent(Laser);
	SegmentLength = 100.0f;*/

}

// Called when the game starts or when spawned
void AHJBullet2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJBullet2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AHJBullet2::FireLaser(FVector& StartPoint, FVector& EndPoint)
//{
//	FHitResult HitResult;
//	FCollisionQueryParams CollisionParams;
//	CollisionParams.AddIgnoredActor(this);
//
//	GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);
//
//	FVector LaserEndPoint = HitResult.bBlockingHit ? HitResult.Location : EndPoint;
//
//	if (LaserBeamEffect)
//	{
//		LaserBeamPSC->SetTemplate(LaserBeamEffect);
//		LaserBeamPSC->SetBeamSourcePoint(0, StartPoint, 0);
//		LaserBeamPSC->SetBeamTargetPoint(0, LaserEndPoint, 0);
//		LaserBeamPSC->ActivateSystem();
//	}
//}

//void AHJBullet2::UpdateSplineMesh()
//{
//	// 기존 스플라인 메쉬 제거
//	for (int i = 0; i < SplineMeshComponents.Num(); ++i)
//	{
//		if (SplineMeshComponents[i])
//		{
//			SplineMeshComponents[i]->ConditionalBeginDestroy();
//		}
//	}
//	SplineMeshComponents.Empty();
//
//	// 필요한 세그멘트 계산 
//	float Distance = Laser->GetSplineLength();
//	int NumSegments = FMath::CeilToInt(Distance / SegmentLength);
//
//	for (int i = 0; i < NumSegments; ++i)
//	{
//		FVector StartPos, StartTangent, EndPos, EndTangent;
//		Laser->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
//		Laser->GetLocationAndTangentAtSplinePoint(i+1, EndPos, EndTangent, ESplineCoordinateSpace::Local);
//
//		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
//		SplineMesh->SetStaticMesh(LaserMesh);
//		SplineMesh->SetMaterial(0, LaserMaterial);
//		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
//		SplineMesh->RegisterComponentWithWorld(GetWorld());
//		SplineMesh->AttachToComponent(Laser, FAttachmentTransformRules::KeepRelativeTransform);
//
//		SplineMeshComponents.Add_GetRef(SplineMesh);
//	}
//
//}

