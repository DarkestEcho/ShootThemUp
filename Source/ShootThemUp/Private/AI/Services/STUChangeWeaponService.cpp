// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/STUChangeWeaponService.h"

#include "AIController.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if(const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        if(USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(Controller->GetPawn()))
        {
            if(Probability > 0 && FMath::FRand() <= Probability)
            {
                WeaponComponent->NextWeapon();
            }
        }
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
