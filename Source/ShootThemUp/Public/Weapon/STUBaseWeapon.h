// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class UNiagaraComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();
    
    void ChangeClip();
    
    bool CanReload() const;
    bool IsAmmoEmpty() const;
    bool IsAmmoFull() const;
    bool IsFiring() const;
    
    virtual void Zoom(bool bEnabled);

    FWeaponUIData GetUIData() const;
    FAmmoData GetAmmoData() const;

    bool TryToAddAmmo(int32 ClipsAmount);
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    FName MuzzleSocketName = "MuzzleSocket";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float MaxDeflectionAngle = 86.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    FAmmoData DefaultAmmo{15, 10, false};
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    UNiagaraSystem* MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
    USoundCue* FireSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
    USoundCue* NoAmmoSound;
    
    virtual void BeginPlay() override;

    virtual void MakeShot();
    virtual bool GetTracedData(FVector& TraceStart, FVector& TraceEnd) const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    
    FVector GetMuzzleWorldLocation() const;
    FRotator GetMuzzleWorldRotation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    float GetAngleBetweenMuzzleAndHit(const FVector& HitPoint) const;
    
    bool IsClipEmpty() const;
    void DecreaseAmmo();
    void LogAmmo() const;

    UNiagaraComponent* SpawnMuzzleFX() const;
    
private:
    bool bFireInProgress;
    FAmmoData CurrentAmmo;
};
