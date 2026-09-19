#pragma once
#include "CoreMinimal.h"
#include "Multiplayer/AetherMultiplayerTypes.h"

class AGEOFAETHER_API FAetherMultiplayerService
{
public:
    bool Initialize(const FAetherMultiplayerConfig& InConfig);
    void Shutdown();
    EAetherAdmissionResult RegisterConnection(uint32 ConnectionId, double NowSeconds);
    bool UnregisterConnection(uint32 ConnectionId);
    bool SetAuthenticated(uint32 ConnectionId, bool bAuthenticated);
    bool Heartbeat(uint32 ConnectionId, double NowSeconds);
    EAetherAuthorityResult ConsumeRequest(uint32 ConnectionId, double NowSeconds);
    int32 RemoveTimedOutConnections(double NowSeconds);
    int32 GetConnectionCount() const { return Connections.Num(); }
    int32 GetAuthenticatedCount() const;
    const FAetherMultiplayerConfig& GetConfig() const { return Config; }
    bool IsRunning() const { return bRunning; }
    const FAetherMultiplayerConnection* FindConnection(uint32 ConnectionId) const;
private:
    void RefillBudget(FAetherMultiplayerConnection& Connection, double NowSeconds) const;
    FAetherMultiplayerConfig Config;
    TMap<uint32, FAetherMultiplayerConnection> Connections;
    bool bRunning = false;
};