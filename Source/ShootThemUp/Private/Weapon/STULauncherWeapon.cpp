// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STULauncherWeapon.h"
#include "STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeapon::StartFire()
{
    Super::StartFire();
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (IsClipEmpty())
    {
        if (!IsAmmoEmpty())
        {
            OnClipEmpty.Broadcast(this);
        }

        // use this function instead of PlaySoundAtLocation just for a difference
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;

    if (!GetTracedData(TraceStart, TraceEnd))
    {
        return;
    }

    DecreaseAmmo();

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

    if (ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform))
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }

    SpawnMuzzleFX();

    UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);

    StopFire();
}
