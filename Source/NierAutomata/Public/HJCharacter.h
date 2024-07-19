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

	// 카메라 & 스프링암 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	// 앞뒤좌우 이동 
	void InputHorizontal(float value);
	void InputVertical(float value);
	FVector DashDirection;
	// 가속 구현 
	void StartDash();
	void EndDash();
	// 대시 구현 
	void InputDash();
	int DashDistance = 1000;
	// 점프 
	void InputJump();
	// 카메라 회전 구현 
	void InputTurn(float value);
	void InputLookup(float value);
	// 공격 구현 
	void StartAttack();
	void EndAttack();

	// 레이저 공격 
	void FireLaser(); 

	UPROPERTY(EditAnywhere)
	class AHJBullet2* LaserBeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// 무기 장착 
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

	// 펫 장착 
	class AHJJumpPet* JumpPet;
	class AHJPet* CurrentPet;

	// 무기 이동 
	UFUNCTION(BlueprintCallable)
	void StartSkyAttack();

	UFUNCTION(BlueprintCallable)
	void EndSkyAttack();


};
