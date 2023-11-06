// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UPhysicalMaterial;

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

    bool TryAddHealth(float HealthAmount);
    bool IsHealthFull() const;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
    TMap<UPhysicalMaterial*, float> DamageModifiers;

    virtual void BeginPlay() override;
private:
    float Health = 0.0f;
    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
        AActor* DamageCauser);

    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );

    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser );
    
    void HealUpdate();
    void SetHealth(float NewHealth);

    void PlayCameraShake() const;

    void Killed(const AController* KillerController) const;
    void ApplyDamage(float Damage, AController* InstigatedBy);
    void ReportDamage(float Damage, AController* InstigatedBy);

    float GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName);
};
