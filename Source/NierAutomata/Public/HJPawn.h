// Fill out your copyright notice in the Description page of Project Settings.
// Possess 대상을 HJCharacter로 지정 - Pawn 헤더와 소스파일은 초기화 상태 그대로

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "HJPawn.generated.h"

UCLASS()
class NIERAUTOMATA_API AHJPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHJPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
