// Fill out your copyright notice in the Description page of Project Settings.


#include "JHRotation.h"

// Sets default values for this component's properties
UJHRotation::UJHRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJHRotation::BeginPlay()
{
	Super::BeginPlay();

	if (TargetType == ETargetType::Actor && TargetOwner == nullptr)
	{
		TargetOwner = GetOwner();
	}
		
}


// Called every frame
void UJHRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJHRotation::RotateLockAt()
{
	//	FVector CurrPos = Target->GetActorLocation();
	//	FVector TargetPos = RotateTarget->GetActorLocation();

	//	CurrPos.Z = 0;
	//	TargetPos.Z = 0;

	//	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(CurrPos, TargetPos);
	//	FRotator InterpRot = UKismetMathLibrary::RInterpTo(this->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, InterpSpeed);

	//	SetActorRotation(InterpRot);

}

