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
	// 델리게이트 추가 
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 카메라 위치 설정  
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	// 펫 위치 설정 
	UPROPERTY(EditAnywhere)
	USceneComponent* petPos;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJPet> petFactory;

	// 캐릭터 이동 
	void UpDown(float value);
	void LeftRight(float value);
	// 점프, 대시, 공격 
	void InputJump();
	void InputDash();
	void InputAttack();

	// 카메라 이동 
	void Turn(float value);
	void Lookup(float value);

	// 델리게이트 기능 
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 공격콤보 구현 
	void AttackStartComboState();
	void AttackEndComboState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanNextCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsComboInputOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCombo;

	UPROPERTY(EditAnywhere)
	bool IsAttacking;

	UPROPERTY(EditAnywhere)
	class UHJAnimInstance2* HJAnim;

	// 시간 부여 
	/*float currTime = 0;

	int Count = 0;

	UPROPERTY(EditAnywhere)
	float createTime = 2;*/
};
