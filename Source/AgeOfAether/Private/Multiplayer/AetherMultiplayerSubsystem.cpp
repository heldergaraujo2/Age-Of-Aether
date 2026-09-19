#include "Multiplayer/AetherMultiplayerSubsystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UAetherMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    FAetherMultiplayerConfig Config;
    Service.Initialize(Config);
}

void UAetherMultiplayerSubsystem::Deinitialize()
{
    Service.Shutdown();
    Super::Deinitialize();
}

bool UAetherMultiplayerSubsystem::RegisterConnection(uint32 ConnectionId, double NowSeconds)
{
    return Service.RegisterConnection(ConnectionId, NowSeconds) == EAetherAdmissionResult::Accepted;
}
bool UAetherMultiplayerSubsystem::UnregisterConnection(uint32 ConnectionId) { return Service.UnregisterConnection(ConnectionId); }
bool UAetherMultiplayerSubsystem::SetAuthenticated(uint32 ConnectionId, bool bAuthenticated) { return Service.SetAuthenticated(ConnectionId, bAuthenticated); }
bool UAetherMultiplayerSubsystem::Heartbeat(uint32 ConnectionId, double NowSeconds) { return Service.Heartbeat(ConnectionId, NowSeconds); }
EAetherAuthorityResult UAetherMultiplayerSubsystem::ConsumeRequest(uint32 ConnectionId, double NowSeconds) { return Service.ConsumeRequest(ConnectionId, NowSeconds); }
int32 UAetherMultiplayerSubsystem::RemoveTimedOutConnections(double NowSeconds) { return Service.RemoveTimedOutConnections(NowSeconds); }
int32 UAetherMultiplayerSubsystem::GetConnectionCount() const { return Service.GetConnectionCount(); }
int32 UAetherMultiplayerSubsystem::GetAuthenticatedCount() const { return Service.GetAuthenticatedCount(); }
const FAetherMultiplayerConfig& UAetherMultiplayerSubsystem::GetConfig() const { return Service.GetConfig(); }