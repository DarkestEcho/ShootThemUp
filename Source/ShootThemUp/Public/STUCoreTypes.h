#pragma once

#include "STUCoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

class ASTUBaseWeapon;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo", meta=(EditCondition = "!bInfinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
    bool bInfinite;
    
};


USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UAnimMontage* ReloadAnimMontage;
};

