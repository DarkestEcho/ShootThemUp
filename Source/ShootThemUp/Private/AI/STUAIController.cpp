// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STUAIController.h"

#include "STUAICharacter.h"
#include "STUAIPerceptionComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
 
    if(const ASTUAICharacter* AICharacter = Cast<ASTUAICharacter>(InPawn))
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AActor* AimActor = STUAIPerceptionComponent->GetClosestEnemy();
    SetFocus(AimActor);
}