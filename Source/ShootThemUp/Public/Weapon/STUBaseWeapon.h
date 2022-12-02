// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"


UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    virtual void Fire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float MaxDeflectionAngle = 86.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    float DamageAmount = 10.0f;
    
    virtual void BeginPlay() override;

    void MakeShot();

    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    bool GetTracedData(FVector& TraceStart, FVector& TraceEnd) const;
    
    FVector GetMuzzleWorldLocation() const;
    FRotator GetMuzzleWorldRotation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    float GetAngleBetweenMuzzleAndHit(const FVector& HitPoint) const;

    void MakeDamage(const FHitResult& HitResult);
};
