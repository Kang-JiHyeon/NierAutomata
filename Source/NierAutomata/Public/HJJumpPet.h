// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HJJumpPet.generated.h"

UCLASS()
class NIERAUTOMATA_API AHJJumpPet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJJumpPet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 무기 매쉬 
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* JumpPet;

};
