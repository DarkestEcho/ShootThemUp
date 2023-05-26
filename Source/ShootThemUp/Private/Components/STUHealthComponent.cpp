// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUHealthComponent.h"

#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());
    check(GetOwner());
    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    if (AActor* ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

float USTUHealthComponent::GetHealth() const
{
    return Health;
}

bool USTUHealthComponent::TryAddHealth(float HealthAmount)
{
    if(IsDead() || IsHealthFull())
    {
        return false;
    }

    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool USTUHealthComponent::IsDead() const
{
    return FMath::IsNearlyZero(Health);
}

float USTUHealthComponent::GetHealthPercent() const
{
    return Health / MaxHealth;
}


void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    SetHealth(Health - Damage);

    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (bAutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %.2f from %s to %s"), Damage, DamageCauser ? *DamageCauser->GetName() : *FString("Undefine"), *GetOwner()->GetName());
    PlayCameraShake();
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const float NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const float HealthDelta = NextHealth - Health;
    
    Health = NextHealth;
    
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake() const
{
    if (IsDead())
    {
        return;
    }

    if(const APawn* Player = GetOwner<APawn>())
    {
        if(const APlayerController* Controller = Player->GetController<APlayerController>())
        {
            if(Controller->PlayerCameraManager)
            {
                Controller->PlayerCameraManager->StartCameraShake(CameraShake);
            }
        }
    }
}

void USTUHealthComponent::Killed(const AController* KillerController) const
{
    const APawn* Player = Cast<APawn>(GetOwner());
    const AController* VictimController = Player->Controller;
    
    ASTUGameModeBase::Killed(KillerController, VictimController);
}
