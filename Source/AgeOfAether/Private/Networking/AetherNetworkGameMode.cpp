#include "Networking/AetherNetworkGameMode.h"

#include "Networking/AetherNetworkGameState.h"
#include "Networking/AetherNetworkPlayerController.h"

AAetherNetworkGameMode::AAetherNetworkGameMode()
{
    GameStateClass = AAetherNetworkGameState::StaticClass();
    PlayerControllerClass = AAetherNetworkPlayerController::StaticClass();
}
