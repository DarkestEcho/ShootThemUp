// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STUAIController.h"

#include "STUAICharacter.h"

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
 
    if(const ASTUAICharacter* AICharacter = Cast<ASTUAICharacter>(InPawn))
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}
