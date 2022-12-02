// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
    UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"));    
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
