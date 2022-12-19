// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/STUAmmoPickup.h"

#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::TryGivePickupTo(APawn* PlayerPawn)
{
    const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(PlayerPawn);
    if(!HealthComponent || HealthComponent->IsDead())
    {
        return false;
    }

    if(USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(PlayerPawn))
    {
        return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
    }
    
    return false;
}
