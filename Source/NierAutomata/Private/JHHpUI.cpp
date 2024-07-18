// Fill out your copyright notice in the Description page of Project Settings.


#include "JHHpUI.h"
#include "Components/ProgressBar.h"

void UJHHpUI::UpdateHPBar(float CurHP, float MaxHP)
{
    CurPercent = CurHP / MaxHP;

    HPBar->SetPercent(CurPercent);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UJHHpUI::UpdateDelayHPBar, 0.5f, false);
}

void UJHHpUI::UpdateDelayHPBar()
{
    DelayHPBar->SetPercent(CurPercent);
}

