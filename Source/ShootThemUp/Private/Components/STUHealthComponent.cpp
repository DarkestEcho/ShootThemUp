// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    check(GetOwner());

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

    if(AActor* ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

float USTUHealthComponent::GetHealth() const
{
    return Health;
}

bool USTUHealthComponent::IsDead() const
{
    return Health <= 0.0f;
}


void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if(Damage <=0.0f || IsDead())
    {
        return;
    }
    
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
    
    if(IsDead())
    {
        OnDeath.Broadcast();
    }
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %.2f from %s to %s"), Damage, DamageCauser ? *DamageCauser->GetName() : *FString("Undefine"), *GetOwner()->GetName());
}
