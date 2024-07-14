// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JHEnemyDamageUI.generated.h"

UCLASS()
class NIERAUTOMATA_API UJHEnemyDamageUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* TextDamage;

	UPROPERTY(EditAnywhere)
	float DelayTime = 1.5f;

private:
	UPROPERTY()
	APlayerController* PlayerController;

	FTimerHandle TimerHandle;
	FVector TracePos;

	UPROPERTY(EditAnywhere)
	float TweenDuration = 0.5f;
	UPROPERTY(EditAnywhere)
	FVector2D TweenOffset = FVector2D(0, -50);

	FVector RandomOffset;
	FVector2D TweenStartLocation;
	FVector2D TweenEndLocation;
	float ElapsedTime;
	bool bIsTweening;
	


public:
	void SetTextDamage(int32 Value);
	void SetTracePosision(FVector Value);
	void DestoryWidget();

	void TweeningLocation();

};

