#include "Networking/AetherNetworkGameMode.h"

#include "Networking/AetherNetworkGameState.h"
#include "Networking/AetherNetworkPlayerController.h"
#include "Characters/AetherCharacter.h"
#include "Characters/AetherCharacterPlayerState.h"
#include "GameFramework/PlayerStart.h"

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

    RestartPlayer(PlayerController);

    AAetherCharacter* SpawnedCharacter = Cast<AAetherCharacter>(PlayerController->GetPawn());
    if (!SpawnedCharacter)
    {
        return;
    }

    if (AAetherCharacterPlayerState* CharacterState = PlayerController->GetPlayerState<AAetherCharacterPlayerState>())
    {
        CharacterState->SetCharacterIdentity(Character);
    }

    SpawnedCharacter->SetActorLocation(Character.WorldLocation);
    SpawnedCharacter->SetActorRotation(Character.WorldRotation);
}
