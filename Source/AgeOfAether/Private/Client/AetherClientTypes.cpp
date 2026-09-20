#include "Client/AetherClientTypes.h"

void FAetherClientIdentitySnapshot::Reset()
{
    AccountID.Reset();
    SessionID.Reset();
    CharacterID.Reset();
    CharacterName.Reset();
    CharacterLevel = 0;
    MapID.Reset();
    ZoneID.Reset();
}

bool FAetherClientIdentitySnapshot::HasAccount() const { return !AccountID.TrimStartAndEnd().IsEmpty() && !SessionID.TrimStartAndEnd().IsEmpty(); }
bool FAetherClientIdentitySnapshot::HasCharacter() const { return HasAccount() && !CharacterID.TrimStartAndEnd().IsEmpty(); }

bool FAetherClientRequestState::IsExpired(double NowSeconds) const
{
    return State == EAetherClientRequestState::Pending
        && TimeoutSeconds > 0.0
        && NowSeconds >= CreatedAtSeconds + TimeoutSeconds;
}

void FAetherClientAssetBinding::Reset()
{
    AssetID.Reset();
    RuntimePath.Reset();
    bResolved = false;
}

void FAetherClientRuntimeSnapshot::Reset()
{
    ConnectionState = EAetherClientConnectionState::Offline;
    Screen = EAetherClientScreen::None;
    Identity.Reset();
    ServerRevision = 0;
    LastServerTimeSeconds = 0.0;
}
