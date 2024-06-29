// Fill out your copyright notice in the Description page of Project Settings.


#include "JHLaserBeamSkill.h"
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

	Params.AddIgnoredActor(GetOwner());
	
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	
}


// Called every frame
void UJHLaserBeamSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OnAttack();

	
}

void UJHLaserBeamSkill::OnInitialize()
{
}

void UJHLaserBeamSkill::OnAttack()
{
	// Laycast로 Hit한 대상까지 발사하고 싶다.
	StartPos = GetComponentLocation();

	for (int32 i = 0; i < MaxCount; i++)
	{
		EndPos = StartPos + GetPositionOnCircle(Distance, i * (360 / MaxCount), StartPos);

		// Hit 대상이 있을 경우
		if (GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos, ObjectParams, Params))
		{
			// todo : 플레이어 감지 처리

			DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, 0.01f, 0, 5.0f);
		}
		// Hit 대상이 없을 경우
		else
		{
			DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 0.01f, 0, 5.0f);
		}

		CreateLine();
	}
}

FVector UJHLaserBeamSkill::GetPositionOnCircle(float Radius, float Degree, FVector CenterPos)
{
	float Radian = FMath::DegreesToRadians(Degree);
	float X = CenterPos.X + Radius * FMath::Cos(Radian);
	float Y = CenterPos.Y + Radius * FMath::Sin(Radian);

	return FVector(X, Y, CenterPos.Z);
}

void UJHLaserBeamSkill::CreateLine()
{
	if (!LineMesh || !LineMaterial) return;

	//for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints() - 1; i++)
	//{

	//	FVector StartTangent, EndTangent;
	//	SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
	//	SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

	//	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
	//	SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//	SplineMeshComponent->RegisterComponent();

	//}


	FVector Start = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector End = SplineComponent->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::Local);

	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
	SplineMeshComponent->SetStaticMesh(LineMesh);
	SplineMeshComponent->SetMaterial(0, LineMaterial);


	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);

	UE_LOG(LogTemp, Warning, TEXT("Create Line!"));
}



