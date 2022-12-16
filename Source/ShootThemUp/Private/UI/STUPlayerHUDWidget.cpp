// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPlayerHUDWidget.h"

#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if (const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent->GetHealthPercent();
    }
    return 0.0f;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (const USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetWeaponUIData(UIData);
    }
    return false;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (const USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetWeaponAmmoData(AmmoData);
    }
    return false;
} 

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const APlayerController* PlayerController = GetOwningPlayer();
    return PlayerController && PlayerController->GetStateName() == NAME_Spectating;
}