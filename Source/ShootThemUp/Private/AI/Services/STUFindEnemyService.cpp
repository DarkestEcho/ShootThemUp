// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUAIPerceptionComponent.h"
#include "STUUtils.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if(UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
    {
        const AAIController* Controller = OwnerComp.GetAIOwner();
        if(const USTUAIPerceptionComponent* PerceptionComponent = STUUtils::GetComponentFromActor<USTUAIPerceptionComponent>(Controller))
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
