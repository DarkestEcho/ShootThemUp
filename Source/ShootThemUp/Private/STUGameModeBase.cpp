// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameModeBase.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "STUGameHUD.h"
#include "STUPlayerState.h"
#include "STURespawnComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();

    SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::Killed(const AController* KillerController, AController* VictimController)
{
    ASTUPlayerState* KillerPlayerState = KillerController ? KillerController->GetPlayerState<ASTUPlayerState>() : nullptr;
    ASTUPlayerState* VictimPlayerState = VictimController ? VictimController->GetPlayerState<ASTUPlayerState>() : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

FGameData ASTUGameModeBase::GetGameData() const
{
    return GameData;
}

int32 ASTUGameModeBase::GetCurrentRoundNum() const
{
    return CurrentRound;
}

int32 ASTUGameModeBase::GetRoundSecondRemaining() const
{
    return RoundCountDown;
}

void ASTUGameModeBase::SpawnBots()
{
    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnParameters);
        RestartPlayer(AIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameRoundUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameRoundUpdate()
{
    // UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    int32 TeamID = 1;
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        AController* Controller = It->Get();
        if (!Controller)
        {
            continue;
        }

        ASTUPlayerState* PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
        if (!PlayerState)
        {
            continue;
        }

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }

    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No Color For Team ID: %i, Set To Default Color: %s"), TeamID,
        *GameData.DefaultTeamColor.ToString())

    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller)
    {
        return;
    }

    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());

    if (!Character)
    {
        return;
    }

    const ASTUPlayerState* PlayerState = Character->GetPlayerState<ASTUPlayerState>();

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo() const
{
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const AController* Controller = It->Get();
        if (!Controller)
        {
            continue;
        }

        const ASTUPlayerState* PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
        if (!PlayerState)
        {
            continue;
        }

        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::StartRespawn(const AController* Controller) const
{
    const bool bRespawnAvailable = RoundCountDown > GameData.MinRoundTimeForRespawn + GameData.RespawnTime;

    if (!bRespawnAvailable)
    {
        return;
    }

    if (USTURespawnComponent* RespawnComponent = STUUtils::GetComponentFromActor<USTURespawnComponent>(Controller))
    {
        RespawnComponent->Respawn(GameData.RespawnTime);
    }
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("====== GAME OVER ======"));
    LogPlayerInfo();

    for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if (MatchState == State)
    {
        return;
    }

    MatchState = State;
    OnMatchStateChange.Broadcast(MatchState);
}

void ASTUGameModeBase::StopAllFire()
{
    for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (USTUWeaponComponent* WeaponComponent = STUUtils::GetComponentFromActor<USTUWeaponComponent>(Pawn))
        {
            WeaponComponent->StopFire();
            WeaponComponent->Zoom(false);
        }
    }
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool bPause = Super::SetPause(PC, CanUnpauseDelegate);

    if (bPause)
    {
        StopAllFire();
        SetMatchState(ESTUMatchState::Pause);
    }

    return bPause;
}

bool ASTUGameModeBase::ClearPause()
{
    const bool bPauseCleared = Super::ClearPause();

    if (bPauseCleared)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }

    return bPauseCleared;
}
