// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

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

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
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
    return CurrentWeapon && !bEquipAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !bEquipAnimInProgress;
}

void USTUWeaponComponent::SpawnWeapons()
{
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        for (const FWeaponData& WeaponData : WeaponsData)
        {
            if (ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass))
            {
                Weapon->SetOwner(Character);
                Weapons.Add(Weapon);

                AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
            }
        }
    }
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }
    
    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if (CurrentWeapon)
        {
            CurrentWeapon->StopFire();
            AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
        }

        CurrentWeapon = Weapons[WeaponIndex];
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
        bEquipAnimInProgress = true;
        PlayAnimMontage(EquipAnimMontage);
    }
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
    if (!EquipAnimMontage)
    {
        return;
    }

    const TArray<FAnimNotifyEvent> NotifyEvents = EquipAnimMontage->Notifies;

    for (const FAnimNotifyEvent& NotifyEvent : NotifyEvents)
    {
        if (USTUEquipFinishedAnimNotify* EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify))
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
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

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent)
    {
        return;
    }

    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}
