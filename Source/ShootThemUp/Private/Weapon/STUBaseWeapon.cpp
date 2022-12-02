// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
    MakeShot();
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    check(WeaponMesh);
}

void ASTUBaseWeapon::MakeShot()
{
    FVector TraceStart;
    FVector TraceEnd;

    if(!GetTracedData(TraceStart, TraceEnd))
    {
        return;
    }
        
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const float AngleBetween = GetAngleBetweenMuzzleAndHit(HitResult.ImpactPoint);
    
    UE_LOG(LogBaseWeapon, Display, TEXT("%f"), AngleBetween);
    
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

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const ACharacter* Player = Cast<ACharacter>(GetOwner());
    return Player? Player->GetController<APlayerController>() : nullptr;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const APlayerController* Controller = GetPlayerController();

    if(!Controller)
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

    if(!GetPlayerViewPoint(ViewLocation, ViewRotation))
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
    
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

float ASTUBaseWeapon::GetAngleBetweenMuzzleAndHit(const FVector& HitPoint) const
{
    return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetMuzzleWorldRotation().Vector(), (HitPoint - GetMuzzleWorldLocation()).GetSafeNormal())));
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
    AActor* DamagedActor = HitResult.GetActor();

    if(!DamagedActor)
    {
        return;
    }

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
