// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STURifleWeapon.h"

#include "Components/STUWeaponFXComponent.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    InitMuzzleFX();
    GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
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

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit && AngleBetween <= MaxDeflectionAngle)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }

    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
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

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(const bool bVisible) const
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!bVisible);
        MuzzleFXComponent->SetVisibility(bVisible);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    if (UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart))
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
