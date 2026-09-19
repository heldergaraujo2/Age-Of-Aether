#include "Networking/AetherNetworkGameState.h"

#include "Net/UnrealNetwork.h"

AAetherNetworkGameState::AAetherNetworkGameState()
{
    bReplicates = true;
    ProtocolVersion = FAetherProtocolVersion::Current();
}

void AAetherNetworkGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAetherNetworkGameState, ProtocolVersion);
    DOREPLIFETIME(AAetherNetworkGameState, AuthoritativeStateRevision);
    DOREPLIFETIME(AAetherNetworkGameState, ConnectedPlayerCount);
    DOREPLIFETIME(AAetherNetworkGameState, bAcceptingConnections);
}

FAetherProtocolVersion AAetherNetworkGameState::GetProtocolVersion() const
{
    return ProtocolVersion;
}

uint64 AAetherNetworkGameState::GetAuthoritativeStateRevision() const
{
    return AuthoritativeStateRevision;
}

void AAetherNetworkGameState::AdvanceAuthoritativeState()
{
    if (!HasAuthority())
    {
        return;
    }

    ++AuthoritativeStateRevision;
}

void AAetherNetworkGameState::SetConnectedPlayerCount(int32 Count)
{
    if (HasAuthority()) ConnectedPlayerCount = FMath::Max(0, Count);
}

void AAetherNetworkGameState::SetAcceptingConnections(bool bAccepting)
{
    if (HasAuthority()) bAcceptingConnections = bAccepting;
}
