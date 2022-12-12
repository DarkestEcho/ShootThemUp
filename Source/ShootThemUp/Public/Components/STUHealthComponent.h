// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();
    
    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsDead() const;
    
    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealthPercent() const;
    
    
    float GetHealth() const;

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(ClampMin = "0.0", ClampMax="1000.0"));
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal")
    bool bAutoHeal = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(ClampMin = "0.01", ClampMax="5.0"))
    float HealUpdateTime = 1.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(ClampMin = "0.0", EditCondition = "bAutoHeal"))
    float HealDelay = 3.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(ClampMin = "0.01", EditCondition = "bAutoHeal"))
    float HealModifier = 5.0f;

    virtual void BeginPlay() override;
private:
    float Health = 0.0f;
    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
        AActor* DamageCauser);

    void HealUpdate();

    void SetHealth(float NewHealth);
};
