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
    if (CurrentWeapon)
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

void USTUWeaponComponent::SpawnWeapons()
{
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        for (const TSubclassOf<ASTUBaseWeapon>& WeaponClass : WeaponClasses)
        {
            if (ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass))
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
    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if (CurrentWeapon)
        {
            CurrentWeapon->StopFire();
            AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
        }

        CurrentWeapon = Weapons[WeaponIndex];
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
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
    if(!EquipAnimMontage)
    {
        return;
    }

    const TArray<FAnimNotifyEvent> NotifyEvents = EquipAnimMontage->Notifies;

    for(const FAnimNotifyEvent& NotifyEvent: NotifyEvents)
    {
        if(USTUEquipFinishedAnimNotify* EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify))
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) const
{
    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()); Character->GetMesh() == MeshComponent)
    {
        UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
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
