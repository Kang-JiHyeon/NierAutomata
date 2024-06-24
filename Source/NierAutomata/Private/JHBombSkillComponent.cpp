// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBombSkillComponent.h"
#include "JHBomb.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UJHBombSkillComponent::UJHBombSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//SetRootComponent(CapsuleComp);
	//// 크기
	//CapsuleComp->SetCapsuleHalfHeight(50);
	//CapsuleComp->SetCapsuleRadius(50);

	// 충돌체
	//capsuleComp->SetCollisionProfileName(TEXT("NoCollision"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (tempMesh.Succeeded())
		MeshComp->SetStaticMesh(tempMesh.Object);
	//meshComp->SetRelativeLocation(FVector(0, 0, -50));

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset02.BasicAsset02'"));
	if (tempMat.Succeeded())
		MeshComp->SetMaterial(0, tempMat.Object);

	ConstructorHelpers::FClassFinder<AJHBomb> tempBomb(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_JHBomb.BP_JHBomb_C'"));
	if (tempBomb.Succeeded())
		BombFactory = tempBomb.Class;

	// 발사 위치
	for (int32 i = 0; i < BombCount; i++) {
		FString FirePosName = FString::Printf(TEXT("FirePos_%d"), i);
		USceneComponent* TempFirePos = CreateDefaultSubobject<USceneComponent>(*FirePosName);

		//TempFirePos->SetupAttachment(CapsuleComp);
		TempFirePos->SetRelativeRotation(FRotator(50, i * (360 / BombCount), 0));

		FirePositions.Add(TempFirePos);
	}
}


// Called when the game starts
void UJHBombSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UJHBombSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	CurrTime += DeltaTime;

	if (CurrTime >= SkillTime) {

		CurrTime = 0;

		Attack();
	}
}


void UJHBombSkillComponent::Attack()
{
	Super::Attack();

	try
	{
		UE_LOG(LogTemp, Warning, TEXT("BombSkill Start!!"));

		if (FirePositions.Num() <= 0) {
			UE_LOG(LogTemp, Warning, TEXT("FirePositions null!!"));
			return;
		}

		if (BombFactory == nullptr) {

			UE_LOG(LogTemp, Warning, TEXT("BombFactory null!!"));
			return;
		}

		for (auto FirePos : FirePositions)
		{
			if (FirePos == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("FirePositions[i] null!!"));
				return;
			}

			GetWorld()->SpawnActor<AJHBomb>(BombFactory, FirePos->GetComponentLocation(), FirePos->GetComponentRotation());

			UE_LOG(LogTemp, Warning, TEXT("Spawn Bomb!!"));
		}

	}
	catch (const std::exception&)
	{
		UE_LOG(LogTemp, Warning, TEXT("BombSkill error!!"));
	}
}
