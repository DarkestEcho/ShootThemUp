// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if(CurrentWeapon)
    {
        CurrentWeapon->StartFire();
    }
}

void USTUWeaponComponent::StopFire()
{
    if(CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if ((CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass)))
        {
            const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
            CurrentWeapon->SetOwner(Character);
        }
    }
}
