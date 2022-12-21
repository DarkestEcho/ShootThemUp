// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/STUHealthPickup.h"

#include "STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::TryGivePickupTo(APawn* PlayerPawn)
{
    if(USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(PlayerPawn))
    {
        return HealthComponent->TryAddHealth(HealthAmount);
    }
    return false;
}
