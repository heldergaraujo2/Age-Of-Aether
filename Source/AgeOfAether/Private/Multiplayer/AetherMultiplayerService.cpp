#include "Multiplayer/AetherMultiplayerService.h"
#include "Math/UnrealMathUtility.h"

bool FAetherMultiplayerService::Initialize(const FAetherMultiplayerConfig& InConfig)
{
    if (!InConfig.IsValid()) return false;
    Config = InConfig;
    Connections.Reset();
    bRunning = true;
    return true;
}

void FAetherMultiplayerService::Shutdown()
{
    Connections.Reset();
    bRunning = false;
}

EAetherAdmissionResult FAetherMultiplayerService::RegisterConnection(uint32 ConnectionId, double NowSeconds)
{
    if (!bRunning || ConnectionId == 0) return EAetherAdmissionResult::InvalidConnection;
    if (Connections.Contains(ConnectionId)) return EAetherAdmissionResult::AlreadyConnected;
    if (Connections.Num() >= Config.MaxPlayers) return EAetherAdmissionResult::ServerFull;
    FAetherMultiplayerConnection Connection;
    Connection.ConnectionId = ConnectionId;
    Connection.ConnectedAt = NowSeconds;
    Connection.LastHeartbeatAt = NowSeconds;
    Connection.LastRequestRefillAt = NowSeconds;
    Connection.RequestBudget = Config.BurstCapacity;
    Connections.Add(ConnectionId, Connection);
    return EAetherAdmissionResult::Accepted;
}

bool FAetherMultiplayerService::UnregisterConnection(uint32 ConnectionId)
{
    return Connections.Remove(ConnectionId) > 0;
}

bool FAetherMultiplayerService::SetAuthenticated(uint32 ConnectionId, bool bAuthenticated)
{
    if (FAetherMultiplayerConnection* Connection = Connections.Find(ConnectionId))
    {
        Connection->bAuthenticated = bAuthenticated;
        return true;
    }
    return false;
}

bool FAetherMultiplayerService::Heartbeat(uint32 ConnectionId, double NowSeconds)
{
    if (FAetherMultiplayerConnection* Connection = Connections.Find(ConnectionId))
    {
        Connection->LastHeartbeatAt = NowSeconds;
        RefillBudget(*Connection, NowSeconds);
        return true;
    }
    return false;
}

void FAetherMultiplayerService::RefillBudget(FAetherMultiplayerConnection& Connection, double NowSeconds) const
{
    const double Elapsed = FMath::Max(0.0, NowSeconds - Connection.LastRequestRefillAt);
    if (Elapsed > 0.0)
    {
        const int32 Refill = FMath::FloorToInt(Elapsed * Config.RequestsPerSecond);
        Connection.RequestBudget = FMath::Min(Config.BurstCapacity, Connection.RequestBudget + Refill);
        Connection.LastRequestRefillAt += static_cast<double>(Refill) / Config.RequestsPerSecond;
    }
}

EAetherAuthorityResult FAetherMultiplayerService::ConsumeRequest(uint32 ConnectionId, double NowSeconds)
{
    FAetherMultiplayerConnection* Connection = Connections.Find(ConnectionId);
    if (!Connection) return EAetherAuthorityResult::InvalidConnection;
    RefillBudget(*Connection, NowSeconds);
    if (Connection->RequestBudget <= 0) return EAetherAuthorityResult::RateLimited;
    --Connection->RequestBudget;
    return Connection->bAuthenticated ? EAetherAuthorityResult::Accepted : EAetherAuthorityResult::NotAuthenticated;
}

int32 FAetherMultiplayerService::RemoveTimedOutConnections(double NowSeconds)
{
    TArray<uint32> Expired;
    for (const TPair<uint32, FAetherMultiplayerConnection>& Pair : Connections)
    {
        if (NowSeconds - Pair.Value.LastHeartbeatAt > Config.HeartbeatTimeoutSeconds)
            Expired.Add(Pair.Key);
    }
    for (uint32 Id : Expired) Connections.Remove(Id);
    return Expired.Num();
}

int32 FAetherMultiplayerService::GetAuthenticatedCount() const
{
    int32 Count = 0;
    for (const TPair<uint32, FAetherMultiplayerConnection>& Pair : Connections)
        Count += Pair.Value.bAuthenticated ? 1 : 0;
    return Count;
}

const FAetherMultiplayerConnection* FAetherMultiplayerService::FindConnection(uint32 ConnectionId) const
{
    return Connections.Find(ConnectionId);
}