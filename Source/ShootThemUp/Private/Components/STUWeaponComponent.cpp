// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STURealoadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if (CanFire())
    {
        CurrentWeapon->StartFire();
    }
}

void USTUWeaponComponent::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip())
    {
        return;
    }
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

void USTUWeaponComponent::Zoom(bool bEnabled)
{
    if(CurrentWeapon)
    {
        CurrentWeapon->Zoom(bEnabled);
    }
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if(CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    if(CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::IsNeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) const
{
    for(const ASTUBaseWeapon* Weapon : Weapons)
    {
        if(Weapon && Weapon->IsA(WeaponType))
        {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}

bool USTUWeaponComponent::IsFiring() const
{
    return CurrentWeapon && CurrentWeapon->IsFiring();
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for(ASTUBaseWeapon* Weapon : Weapons)
    {
        if(Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    checkf(WeaponsData.Num() == WeaponNum, TEXT("Our character can hold onle %i weapon items"), WeaponNum);

    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
    StopFire();
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    CurrentWeapon = nullptr;
    for (ASTUBaseWeapon* Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::SpawnWeapons()
{
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        for (const FWeaponData& WeaponData : WeaponsData)
        {
            if (ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass))
            {
                Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty);
                Weapon->SetOwner(Character);
                Weapons.Add(Weapon);

                AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
            }
        }
    }
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if (CurrentWeapon)
        {
            CurrentWeapon->Zoom(false);
            CurrentWeapon->StopFire();
            AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
        }

        CurrentWeapon = Weapons[WeaponIndex];

        const auto CurrenWeaponData = WeaponsData.FindByPredicate([&](const FWeaponData& Data)
        {
            return Data.WeaponClass == CurrentWeapon->GetClass();
        });
        CurrentReloadAnimMontage = CurrenWeaponData ? CurrenWeaponData->ReloadAnimMontage : nullptr;

        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
        bEquipAnimInProgress = true;
        PlayAnimMontage(EquipAnimMontage);
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (!CanReload())
    {
        return;
    }

    StopFire();
    //CurrentWeapon->ChangeClip();
    bReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) const
{
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        Character->PlayAnimMontage(Animation);
    }
}

void USTUWeaponComponent::InitAnimations()
{
    if (USTUEquipFinishedAnimNotify* EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage))
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip Anim Notify not found"));
        checkNoEntry();
    }

    for (const FWeaponData& WeaponData : WeaponsData)
    {
        if (USTURealoadFinishedAnimNotify* ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTURealoadFinishedAnimNotify>(
            WeaponData.ReloadAnimMontage))
        {
            ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        }
        else
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload Anim Notify not found"));
            checkNoEntry();
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()); Character->GetMesh() == MeshComponent)
    {
        bEquipAnimInProgress = false;
    }
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()); Character->GetMesh() == MeshComponent)
    {
        UE_LOG(LogWeaponComponent, Display, TEXT("OnReloadFinished"));
        CurrentWeapon->ChangeClip();
        bReloadAnimInProgress = false;
    }
}

void USTUWeaponComponent::OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon)
{
    if(!AmmoEmptyWeapon)
    {
        return;
    }
    
    if(CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip();
        return;
    }
    
    for(ASTUBaseWeapon* Weapon : Weapons)
    {
        if(Weapon == AmmoEmptyWeapon)
        {
                Weapon->ChangeClip();
        }
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent)
    {
        return;
    }

    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}
