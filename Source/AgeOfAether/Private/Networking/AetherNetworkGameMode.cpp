#include "Networking/AetherNetworkGameMode.h"

#include "Networking/AetherNetworkGameState.h"
#include "Networking/AetherNetworkPlayerController.h"
#include "Characters/AetherCharacter.h"
#include "Characters/AetherCharacterPlayerState.h"
#include "Characters/AetherCharacterSubsystem.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/GameInstance.h"
#include "World/AetherWorldSubsystem.h"

AAetherNetworkGameMode::AAetherNetworkGameMode()
{
    GameStateClass = AAetherNetworkGameState::StaticClass();
    PlayerControllerClass = AAetherNetworkPlayerController::StaticClass();
    PlayerStateClass = AAetherCharacterPlayerState::StaticClass();
    DefaultPawnClass = AAetherCharacter::StaticClass();
    bStartPlayersAsSpectators = true;
}


void AAetherNetworkGameMode::SpawnSelectedCharacter(
    APlayerController* PlayerController,
    const FAetherCharacterRecord& Character)
{
    if (!HasAuthority() || !PlayerController)
    {
        return;
    }

    if (PlayerController->GetPawn())
    {
        PlayerController->GetPawn()->Destroy();
    }

    FAetherCharacterRecord SpawnCharacter = Character;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UAetherWorldSubsystem* World = GameInstance->GetSubsystem<UAetherWorldSubsystem>())
        {
            FAetherWorldTransitionResult WorldResult;
            if (World->InitializeCharacterSpawn(
                Character.AccountId,
                Character.CharacterId,
                WorldResult))
            {
                FAetherCharacterRecord RefreshedCharacter;
                if (WorldResult.IsAccepted())
                {
                    if (UAetherCharacterSubsystem* Characters =
                        GameInstance->GetSubsystem<UAetherCharacterSubsystem>())
                    {
                        if (Characters->FindCharacter(Character.CharacterId, RefreshedCharacter))
                        {
                            SpawnCharacter = RefreshedCharacter;
                        }
                    }
                }
            }
        }
    }

    RestartPlayer(PlayerController);

    AAetherCharacter* SpawnedCharacter = Cast<AAetherCharacter>(PlayerController->GetPawn());
    if (!SpawnedCharacter)
    {
        return;
    }

    if (AAetherCharacterPlayerState* CharacterState = PlayerController->GetPlayerState<AAetherCharacterPlayerState>())
    {
        CharacterState->SetCharacterIdentity(SpawnCharacter);
    }

    SpawnedCharacter->InitializeCharacterIdentity(SpawnCharacter.CharacterId);
    SpawnedCharacter->SetActorLocation(SpawnCharacter.WorldLocation);
    SpawnedCharacter->SetActorRotation(SpawnCharacter.WorldRotation);
}
