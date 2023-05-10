 // Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/STUHealthPercentDecorator.h"

#include "AIController.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"

 USTUHealthPercentDecorator::USTUHealthPercentDecorator()
 {
     NodeName = "Health Percent";
 }

 bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
 {
     const AAIController* Controller = OwnerComp.GetAIOwner();

     if(!Controller)
     {
         return false;
     }

     const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(Controller->GetPawn());

     if(!HealthComponent || HealthComponent->IsDead())
     {
         return false;
     }

     return HealthComponent->GetHealthPercent() <= HealthPercent;     
 }
