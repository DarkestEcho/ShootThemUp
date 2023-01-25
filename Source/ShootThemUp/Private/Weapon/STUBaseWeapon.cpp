// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeShot()
{
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const ACharacter* Player = Cast<ACharacter>(GetOwner());
    return Player ? Player->GetController<APlayerController>() : nullptr;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const APlayerController* Controller = GetPlayerController();

    if (!Controller)
    {
        return false;
    }

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

bool ASTUBaseWeapon::GetTracedData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FRotator ASTUBaseWeapon::GetMuzzleWorldRotation() const
{
    return WeaponMesh->GetSocketRotation(MuzzleSocketName);
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

float ASTUBaseWeapon::GetAngleBetweenMuzzleAndHit(const FVector& HitPoint) const
{
    return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetMuzzleWorldRotation().Vector(),
        (HitPoint - GetMuzzleWorldLocation()).GetSafeNormal())));
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (IsClipEmpty())
    {
        UE_LOG(LogBaseWeapon, Error, TEXT("Clip is empty"));
        checkNoEntry();
        return;
    }

    --CurrentAmmo.Bullets;
    //LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.bInfinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
            return;
        }
        --CurrentAmmo.Clips;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && (CurrentAmmo.bInfinite || CurrentAmmo.Clips > 0);
}

FWeaponUIData ASTUBaseWeapon::GetUIData() const
{
    return UIData;
}

FAmmoData ASTUBaseWeapon::GetAmmoData() const
{
    return CurrentAmmo;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.bInfinite || IsAmmoFull() || ClipsAmount <= 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Ammo was empty"));
        return false;
    }

    if (IsAmmoEmpty())
    {
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const int32 NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(LogBaseWeapon, Warning, TEXT("Clips were added"));
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
            UE_LOG(LogBaseWeapon, Warning, TEXT("Ammo is full now"));
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        UE_LOG(LogBaseWeapon, Warning, TEXT("Bullets were added"));
    }

    return true;
}

void ASTUBaseWeapon::LogAmmo() const
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() const
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, //
        WeaponMesh,                                        //
        MuzzleSocketName,                                  //
        FVector::ZeroVector,                               //
        FRotator::ZeroRotator,                             //
        EAttachLocation::SnapToTarget,                     //
        true);
}
