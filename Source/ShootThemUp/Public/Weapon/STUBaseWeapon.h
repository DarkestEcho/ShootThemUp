// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

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
    
    virtual void BeginPlay() override;

    virtual void MakeShot();
    virtual bool GetTracedData(FVector& TraceStart, FVector& TraceEnd) const;

    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    
    FVector GetMuzzleWorldLocation() const;
    FRotator GetMuzzleWorldRotation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    float GetAngleBetweenMuzzleAndHit(const FVector& HitPoint) const;

    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void DecreaseAmmo();
    void LogAmmo() const;

private:
    FAmmoData CurrentAmmo;
};
