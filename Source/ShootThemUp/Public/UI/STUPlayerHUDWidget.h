// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;
class USTUHealthComponent;
class USTUWeaponComponent;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    
    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
    
    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerSpectating() const;

    UFUNCTION(BlueprintImplementableEvent, Category="UI")
    void OnTakeDamage();

    UFUNCTION(BlueprintCallable, Category="UI")
    int32 GetKillsNum() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    FString FormatBullets(int32 BulletsNum) const;
        
protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    float PercentColorThreshold = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    FLinearColor GoodColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    FLinearColor BadColor = FLinearColor::Red;
    
    virtual void NativeOnInitialized() override;

private:
    void OnHealthChanged(float Health, float HealthDelta);
    void OnNewPawn(APawn* NewPawn);
    void UpdateHealthBar();
};
