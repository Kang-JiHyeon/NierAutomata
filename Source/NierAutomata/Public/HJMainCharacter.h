// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "HJMainCharacter.generated.h"
UCLASS()
class NIERAUTOMATA_API AHJMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHJMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USceneComponent* petPos;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJPet> petFactory;

	void UpDown(float value);
	void LeftRight(float value);
	void InputJump();

	float u;
	float r;

	int speed = 800;

};
