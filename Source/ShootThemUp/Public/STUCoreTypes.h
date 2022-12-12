#pragma once

#include "STUCoreTypes.generated.h"

// Weapon
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

// Health
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

// Weapon
#pragma region Weapon
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
#pragma endregion Weapon

// UI
#pragma region UI
USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    UTexture2D* CrossHairIcon;
};
#pragma endregion UI