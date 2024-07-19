// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "HJWeapon2.h"

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
	int DashDistance = 1000;
	// ���� 
	void InputJump();
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// ���� ���� 
	class AHJWeapon* CurrentWeapon;
	class AHJWeapon2* CurrentWeapon2;
	void WeaponEquip();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHJWeapon> WeaponFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHJWeapon2> WeaponFactory2;

	UFUNCTION(BlueprintCallable)
	void StartWeapon();
	
	UFUNCTION(BlueprintCallable)
	void BackWeapon();

	// �� ���� 
	class AHJJumpPet* JumpPet;
	class AHJPet* CurrentPet;

	// ���� �̵� 
	UFUNCTION(BlueprintCallable)
	void StartSkyAttack();

	UFUNCTION(BlueprintCallable)
	void EndSkyAttack();


};
