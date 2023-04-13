// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}


EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

    if (!Controller || !Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = Controller->GetPawn();

    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);

    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation NavLocation;
    FVector PawnLocation = Pawn->GetActorLocation();
    if (!bSelfCenter)
    {
        const AActor* CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor)
        {
            return EBTNodeResult::Failed;
        }
        
        PawnLocation = CenterActor->GetActorLocation();
    }

    const bool bFound = NavSys->GetRandomReachablePointInRadius(PawnLocation, Radius, NavLocation);

    if (!bFound)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}

/*
EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (const AAIController* Controller = OwnerComp.GetAIOwner(); UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
    {
        if (APawn* Pawn = Controller->GetPawn())
        {
            if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn))
            {
                FNavLocation NavLocation;

                if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavLocation))
                {
                    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
                    
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
*/
