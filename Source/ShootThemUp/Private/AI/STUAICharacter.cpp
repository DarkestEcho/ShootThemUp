// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STUAICharacter.h"

#include "BrainComponent.h"
#include "STUAIController.h"
#include "STUAIWeaponComponent.h"
#include "STUHealthBarWidget.h"
#include "STUHealthComponent.h"
#include "Components/WidgetComponent.h"
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

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthWidgetComponent);
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

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta) const
{
    Super::OnHealthChanged(Health, HealthDelta);

    if(const USTUHealthBarWidget* HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject()))
    {
        HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
    }
    
}

void ASTUAICharacter::UpdateHealthWidgetVisibility() const
{
    if(!GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
    {
        return;
    }

    const FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const float Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
