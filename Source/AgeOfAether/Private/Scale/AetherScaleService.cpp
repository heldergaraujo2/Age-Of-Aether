#include "Scale/AetherScaleService.h"
#include "Math/UnrealMathUtility.h"

bool FAetherScaleService::Initialize(const FAetherScaleConfig& InConfig, const FAetherServerNodeId& InNodeId, double NowSeconds)
{
    if (!InConfig.IsValid() || !InNodeId.IsValid()) return false;
    Config = InConfig;
    Node = FAetherServerNodeDescriptor();
    Node.NodeId = InNodeId;
    Node.MaxPlayers = FMath::Min(InConfig.MaxPlayers, InConfig.MaxConnectionsPerShard);
    Node.Lifecycle = EAetherServerLifecycle::Accepting;
    Node.LastHeartbeatAt = NowSeconds;
    Load = FAetherServerLoadSnapshot();
    Load.MaxPlayers = Node.MaxPlayers;
    NextTransferId = 1;
    bRunning = true;
    return true;
}

void FAetherScaleService::Shutdown()
{
    Node.Lifecycle = EAetherServerLifecycle::Stopped;
    Node.bHealthy = false;
    bRunning = false;
}

EAetherScaleResult FAetherScaleService::RegisterPlayer(double NowSeconds)
{
    if (!bRunning) return EAetherScaleResult::ServerUnhealthy;
    if (Node.Lifecycle == EAetherServerLifecycle::Draining) return EAetherScaleResult::ServerDraining;
    if (Node.Lifecycle != EAetherServerLifecycle::Accepting) return EAetherScaleResult::ServerUnhealthy;
    if (!Node.bHealthy || !IsHealthy(NowSeconds)) return EAetherScaleResult::ServerUnhealthy;
    if (Node.CurrentPlayers >= Node.MaxPlayers) return EAetherScaleResult::ServerFull;
    ++Node.CurrentPlayers;
    Load.ActiveConnections = Node.CurrentPlayers;
    Load.AuthenticatedPlayers = FMath::Min(Load.AuthenticatedPlayers + 1, Load.ActiveConnections);
    return EAetherScaleResult::Accepted;
}

bool FAetherScaleService::UnregisterPlayer()
{
    if (Node.CurrentPlayers <= 0) return false;
    --Node.CurrentPlayers;
    Load.ActiveConnections = Node.CurrentPlayers;
    Load.AuthenticatedPlayers = FMath::Min(Load.AuthenticatedPlayers, Load.ActiveConnections);
    return true;
}

EAetherScaleResult FAetherScaleService::UpdateLoad(const FAetherServerLoadSnapshot& InSnapshot, double NowSeconds)
{
    if (!bRunning) return EAetherScaleResult::ServerUnhealthy;
    if (!InSnapshot.IsValid() || InSnapshot.MaxPlayers != Node.MaxPlayers)
        return EAetherScaleResult::InvalidConfiguration;
    Load = InSnapshot;
    Node.CurrentPlayers = InSnapshot.ActiveConnections;
    Node.LastHeartbeatAt = NowSeconds;
    Node.bHealthy = true;
    if (GetLoadScore() >= Config.HardLoadThreshold) Node.Lifecycle = EAetherServerLifecycle::Draining;
    return EAetherScaleResult::Accepted;
}

EAetherScaleResult FAetherScaleService::BeginDrain()
{
    if (!bRunning) return EAetherScaleResult::ServerUnhealthy;
    Node.Lifecycle = EAetherServerLifecycle::Draining;
    return EAetherScaleResult::Accepted;
}

EAetherScaleResult FAetherScaleService::ResumeAccepting()
{
    if (!bRunning || !Node.bHealthy) return EAetherScaleResult::ServerUnhealthy;
    Node.Lifecycle = EAetherServerLifecycle::Accepting;
    return EAetherScaleResult::Accepted;
}

bool FAetherScaleService::CanAcceptConnection() const
{
    return bRunning && Node.bHealthy && Node.Lifecycle == EAetherServerLifecycle::Accepting &&
        Node.CurrentPlayers < Node.MaxPlayers;
}

bool FAetherScaleService::IsHealthy(double NowSeconds) const
{
    return bRunning && Node.bHealthy &&
        (NowSeconds - Node.LastHeartbeatAt) <= Config.NodeHeartbeatTimeoutSeconds;
}

bool FAetherScaleService::IsDraining() const { return Node.Lifecycle == EAetherServerLifecycle::Draining; }

float FAetherScaleService::GetLoadScore() const
{
    const float Connection = Load.ConnectionLoad();
    const float Tick = Config.TargetServerTickRate > 0.0f ?
        static_cast<float>(Load.ServerTickMilliseconds / (1000.0 / Config.TargetServerTickRate)) : 1.0f;
    const float Replication = FMath::Clamp(static_cast<float>(Load.ReplicationMilliseconds / 16.6667), 0.0f, 2.0f);
    return FMath::Max3(Connection, FMath::Clamp(Tick, 0.0f, 2.0f), Replication);
}

EAetherScaleResult FAetherScaleService::CreateTransferRequest(
    const FString& AccountId, const FString& CharacterId, const FAetherServerNodeId& DestinationNode,
    const FString& TargetZoneId, double NowSeconds, FAetherServerTransferRequest& OutRequest)
{
    if (!bRunning || AccountId.TrimStartAndEnd().IsEmpty() || CharacterId.TrimStartAndEnd().IsEmpty() ||
        !DestinationNode.IsValid() || TargetZoneId.TrimStartAndEnd().IsEmpty())
        return EAetherScaleResult::InvalidTransfer;
    if (DestinationNode == Node.NodeId) return EAetherScaleResult::InvalidTransfer;
    OutRequest = FAetherServerTransferRequest();
    OutRequest.TransferId = NextTransferId++;
    OutRequest.AccountId = AccountId;
    OutRequest.CharacterId = CharacterId;
    OutRequest.SourceNode = Node.NodeId;
    OutRequest.DestinationNode = DestinationNode;
    OutRequest.TargetZoneId = TargetZoneId;
    OutRequest.CreatedAt = NowSeconds;
    OutRequest.ExpiresAt = NowSeconds + Config.TransferTimeoutSeconds;
    return EAetherScaleResult::Accepted;
}

EAetherScaleResult FAetherScaleService::ValidateTransferRequest(
    const FAetherServerTransferRequest& Request, double NowSeconds) const
{
    if (!bRunning || Request.TransferId == 0 || !Request.SourceNode.IsValid() ||
        !Request.DestinationNode.IsValid() || Request.SourceNode == Request.DestinationNode)
        return EAetherScaleResult::InvalidTransfer;
    if (Request.ExpiresAt < NowSeconds || Request.ExpiresAt < Request.CreatedAt)
        return EAetherScaleResult::TransferExpired;
    return EAetherScaleResult::Accepted;
}
