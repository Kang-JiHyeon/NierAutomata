// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JHHpUI.generated.h"

/**
 * 
 */
UCLASS()
class NIERAUTOMATA_API UJHHpUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HPBar;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* DelayHPBar;

	FTimerHandle TimerHandle;
	float CurPercent;

	UFUNCTION(BlueprintCallable)
	void UpdateHPBar(float CurHP, float MaxHP);
	UFUNCTION(BlueprintCallable)
	void UpdateDelayHPBar();
};
