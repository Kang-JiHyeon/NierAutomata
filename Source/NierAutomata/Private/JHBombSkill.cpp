// Fill out your copyright notice in the Description page of Project Settings.


#include "JHBombSkill.h"
#include "JHBomb.h"
#include "JHEnemy.h"

// Sets default values for this component's properties
UJHBombSkill::UJHBombSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SuperFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("Fire Position"));


	// 발사 위치
	for (int32 i = 0; i < BombCount; i++) {
		FString FirePosName = FString::Printf(TEXT("FirePos_%d"), i);
		USceneComponent* TempFirePos = CreateDefaultSubobject<USceneComponent>(*FirePosName);

		TempFirePos->SetRelativeRotation(FRotator(50, i * (360 / BombCount), 0));

		TempFirePos->SetupAttachment(SuperFirePos);

		FirePositions.Add(TempFirePos);
	}

	ConstructorHelpers::FClassFinder<AJHBomb> tempBomb(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_JHBomb.BP_JHBomb_C'"));
	if (tempBomb.Succeeded())
		BombFactory = tempBomb.Class;
}


// Called when the game starts
void UJHBombSkill::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<AJHEnemy>(GetOwner());
}


// Called every frame
void UJHBombSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}


void UJHBombSkill::Attack()
{
	//CurrTime += GetWorld()->DeltaTimeSeconds;

	//if (CurrTime >= SkillTime) {

	//	CurrTime = 0;
	//	Fire();
	//}
	Fire();
}

void UJHBombSkill::Fire()
{
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

