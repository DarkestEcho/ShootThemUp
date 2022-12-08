// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"


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

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

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
    void ChangeClip();
    void LogAmmo() const;

private:
    FAmmoData CurrentAmmo;
};
