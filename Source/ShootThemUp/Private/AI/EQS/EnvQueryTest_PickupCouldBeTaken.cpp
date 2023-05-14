// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    bool bWantsToBeTakable = BoolValue.GetValue();
    
    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const ASTUBasePickup* PickupActor = Cast<ASTUBasePickup>(ItemActor);

        if (!PickupActor)
        {
            continue;
        }
        
        const bool bCouldBeTaken = PickupActor->CouldBeTaken();

        It.SetScore(TestPurpose, FilterType, bCouldBeTaken, bWantsToBeTakable);
    }
}
