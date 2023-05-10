// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/STUNeedAmmoDecorator.h"

#include "AIController.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    if(const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        if(const USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(Controller->GetPawn()))
        {
            return WeaponComponent->IsNeedAmmo(WeaponType);
        }
    }

    return false;
}
