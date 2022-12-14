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

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (const USTUWeaponComponent* WeaponComponent = GetWeaponComponent())
    {
        return WeaponComponent->GetWeaponUIData(UIData);
    }
    return false;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (const USTUWeaponComponent* WeaponComponent = GetWeaponComponent())
    {
        return WeaponComponent->GetWeaponAmmoData(AmmoData);
    }
    return false;
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    return STUUtils::GetComponentFromPawn<USTUWeaponComponent>(GetOwningPlayerPawn());
}
