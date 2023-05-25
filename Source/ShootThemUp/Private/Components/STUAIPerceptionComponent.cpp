// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUAIPerceptionComponent.h"

#include "STUAIController.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);

    if (PerceiveActors.Num() == 0)
    {
        return nullptr;
    }

    const AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
    {
        return nullptr;
    }

    const APawn* Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return nullptr; 
    }

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for(AActor* PerceiveActor : PerceiveActors)
    {
        const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(PerceiveActor);

        const APawn* PerceivePawn = Cast<APawn>(PerceiveActor);
        const bool bEnemies = PerceivePawn && STUUtils::AreEnemies(Controller, PerceivePawn->Controller);
        
        if(HealthComponent && !HealthComponent->IsDead() && bEnemies)
        {
            const float CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if(CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = Cast<APawn>(PerceiveActor);
            }
        }
    }
    
    return BestPawn;
}
