// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUProjectile();

    void SetShotDirection(const FVector& Direction);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
    USphereComponent* CollisionComponent;
    
    UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
    UProjectileMovementComponent* MovementComponent;
    
	virtual void BeginPlay() override;
private:
    FVector ShotDirection;
};
