// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STUAICharacter.h"

#include "BrainComponent.h"
#include "STUAIController.h"
#include "STUAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
: Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;

    if(UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        MovementComponent->bUseControllerDesiredRotation = true;
        MovementComponent->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    if(const AAIController* STUController = Cast<AAIController>(Controller))
    {
        if(STUController->BrainComponent)
        {
            STUController->BrainComponent->Cleanup();
        }
    }
}
