// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/STUFireService.h"

#include "AIController.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

    const bool bHasAim = Blackboard && Blackboard->GetValueAsObject(EnemyKeyActor.SelectedKeyName);

    if(const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        if(USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(Controller->GetPawn()))
        {
            bHasAim? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }        
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
