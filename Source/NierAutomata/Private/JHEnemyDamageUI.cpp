// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemyDamageUI.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// 생성됬을 때 위로 올라간다.
// n초뒤에 파괴된다.

void UJHEnemyDamageUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = GetWorld()->GetFirstPlayerController();
	
	bIsTweening = true;

	// 랜덤 위치 값 설정
	float RandomX = UKismetMathLibrary::RandomFloatInRange(-20, 20);
	float RandomY = UKismetMathLibrary::RandomFloatInRange(-20, 20);
	RandomOffset = FVector(RandomX, RandomY, 0);
	
	// 일정 시간 뒤에 파괴
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UJHEnemyDamageUI::DestoryWidget, DelayTime, false);
}

void UJHEnemyDamageUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerController != nullptr)
	{
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, TracePos + RandomOffset, TweenStartLocation, true);

		TweenEndLocation = TweenStartLocation + TweenOffset;

		if (bIsTweening)
		{
			TweeningLocation();
		}
		else
		{
			SetRenderTranslation(TweenEndLocation);
		}
	}
}

void UJHEnemyDamageUI::SetTextDamage(int32 Value)
{
	TextDamage->SetText(FText::AsNumber(Value));
}

void UJHEnemyDamageUI::SetTracePosision(FVector Value)
{
	TracePos = Value;
}

void UJHEnemyDamageUI::DestoryWidget()
{
	this->RemoveFromParent();
}

void UJHEnemyDamageUI::TweeningLocation()
{
	ElapsedTime += GetWorld()->GetDeltaSeconds();
	float Alpha = ElapsedTime / TweenDuration;
	if (Alpha > 1.0f)
	{
		Alpha = 1.0f;
		bIsTweening = false;
	}
	FVector2D NewLocation = FMath::Lerp(TweenStartLocation, TweenEndLocation, Alpha);
	SetRenderTranslation(NewLocation);
}
