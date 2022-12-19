// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STURifleWeapon.h"

#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

void ASTURifleWeapon::StartFire()
{
    GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
    if (IsClipEmpty())
    {
        StopFire();
        if (!IsAmmoEmpty())
        {
            OnClipEmpty.Broadcast(this);
        }
        return;
    }
    
    FVector TraceStart;
    FVector TraceEnd;

    if (!GetTracedData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    DecreaseAmmo();

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const float AngleBetween = GetAngleBetweenMuzzleAndHit(HitResult.ImpactPoint);

    if (HitResult.bBlockingHit && AngleBetween <= MaxDeflectionAngle)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

bool ASTURifleWeapon::GetTracedData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation;
    const float HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    AActor* DamagedActor = HitResult.GetActor();

    if (!DamagedActor)
    {
        return;
    }

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
