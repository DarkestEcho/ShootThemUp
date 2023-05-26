// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUSpectatorWidget.h"

#include "STURespawnComponent.h"
#include "STUUtils.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    USTURespawnComponent* RespawnComponent = STUUtils::GetComponentFromActor<USTURespawnComponent>(GetOwningPlayer());

    if(!RespawnComponent || !RespawnComponent->IsRespawnInProgress())
    {
        return false;
    }

    CountDownTime = RespawnComponent->GetRespawnCountDown();    
    return true;
}
