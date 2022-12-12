// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPlayerHUDWidget.h"

#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if (const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromPawn<USTUHealthComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent->GetHealthPercent();
    }
    return 0.0f;
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (const USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromPawn<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetWeaponUIData(UIData);
    }
    return false;
}
