// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "HJCharacter.generated.h"

UCLASS()
class NIERAUTOMATA_API AHJCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHJCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ī�޶� & �������� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	// �յ��¿� �̵� 
	void InputHorizontal(float value);
	void InputVertical(float value);
	FVector DashDirection;
	// ���� ���� 
	void StartDash();
	void EndDash();
	// ��� ���� 
	void InputDash();
	int DashDistance = 1500;
	// ���� 
	void InputJump();
	// ���� �߷� (���� �ӵ�) 
	void StartJump();
	void EndJump();
	// ī�޶� ȸ�� ���� 
	void InputTurn(float value);
	void InputLookup(float value);
	// ���� ���� 
	void StartAttack();
	void EndAttack();

	// ������ ���� 
	void FireLaser();

	UPROPERTY(EditAnywhere)
	class AHJBullet2* LaserBeam;

	UPROPERTY()
	// ���� ���� 
	class AHJWeapon* CurrentWeapon;

	UFUNCTION(BlueprintCallable)
	void StartWeapon();
	
	UFUNCTION(BlueprintCallable)
	void BackWeapon();
};
