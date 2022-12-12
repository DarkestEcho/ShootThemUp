// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPlayerHUDWidget.h"

#include "STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if (const APawn* Player = GetOwningPlayerPawn())
    {
        if(const USTUHealthComponent* HealthComponent = Cast<USTUHealthComponent>(Player->GetComponentByClass(USTUHealthComponent::StaticClass())))
        {
            return HealthComponent->GetHealthPercent();
        }
        return 0.0f;
    }
    return 0.0f;
}
