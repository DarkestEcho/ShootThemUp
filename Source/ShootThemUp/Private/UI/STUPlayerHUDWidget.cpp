// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPlayerHUDWidget.h"

#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Navigation/CrowdFollowingComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if (const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent->GetHealthPercent();
    }
    return 0.0f;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (const USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetWeaponUIData(UIData);
    }
    return false;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (const USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetWeaponAmmoData(AmmoData);
    }
    return false;
} 

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const APlayerController* PlayerController = GetOwningPlayer();
    return PlayerController && PlayerController->GetStateName() == NAME_Spectating;
}

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
    if(const APlayerController* Controller = GetOwningPlayer())
    {
        const ASTUPlayerState* PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
        return PlayerState ? PlayerState->GetKillsNum() : 0;
    }
    return 0;
}

FString USTUPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    FString BulletString = FString::FromInt(BulletsNum);
    const int32 SymbolsNumToAdd = MaxLen - BulletString.Len();

    if(SymbolsNumToAdd > 0)
    {
        BulletString = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletString);
    }

    return BulletString;
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if(HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }

    UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    USTUHealthComponent* HealthComponent = STUUtils::GetComponentFromActor<USTUHealthComponent>(NewPawn);
    if (HealthComponent && ! HealthComponent->OnHealthChanged.IsBoundToObject(this))
    {
        HealthComponent->OnHealthChanged.AddUObject(this,&USTUPlayerHUDWidget::OnHealthChanged);
    }

    UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if(HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}
