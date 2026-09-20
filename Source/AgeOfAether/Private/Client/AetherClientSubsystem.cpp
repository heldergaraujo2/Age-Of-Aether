#include "Client/AetherClientSubsystem.h"

void UAetherClientSubsystem::Deinitialize()
{
    ResetClient();
    Super::Deinitialize();
}

void UAetherClientSubsystem::ResetClient()
{
    Requests.Reset();
    NextRequestID = 1;
    const EAetherClientConnectionState OldConnection = Snapshot.ConnectionState;
    const EAetherClientScreen OldScreen = Snapshot.Screen;
    Snapshot.Reset();
    if (OldConnection != Snapshot.ConnectionState) OnConnectionStateChanged.Broadcast(Snapshot.ConnectionState);
    if (OldScreen != Snapshot.Screen) OnScreenChanged.Broadcast(Snapshot.Screen);
}

void UAetherClientSubsystem::SetConnectionState(EAetherClientConnectionState NewState)
{
    if (Snapshot.ConnectionState == NewState) return;
    Snapshot.ConnectionState = NewState;
    OnConnectionStateChanged.Broadcast(NewState);
}

void UAetherClientSubsystem::SetScreen(EAetherClientScreen NewScreen)
{
    if (Snapshot.Screen == NewScreen) return;
    Snapshot.Screen = NewScreen;
    OnScreenChanged.Broadcast(NewScreen);
}

uint32 UAetherClientSubsystem::BeginRequest(const FString& Operation, double NowSeconds, double TimeoutSeconds)
{
    if (NextRequestID == 0) NextRequestID = 1;
    const uint32 ID = NextRequestID++;
    FAetherClientRequestState Request;
    Request.RequestID = ID;
    Request.State = EAetherClientRequestState::Pending;
    Request.CreatedAtSeconds = NowSeconds;
    Request.TimeoutSeconds = FMath::Max(0.001, TimeoutSeconds);
    Request.Operation = Operation.TrimStartAndEnd();
    Requests.Add(ID, Request);
    OnRequestChanged.Broadcast(Request);
    return ID;
}

bool UAetherClientSubsystem::CompleteRequest(uint32 RequestID, bool bAccepted)
{
    FAetherClientRequestState* Request = Requests.Find(RequestID);
    if (!Request || Request->State != EAetherClientRequestState::Pending) return false;
    Request->State = bAccepted ? EAetherClientRequestState::Accepted : EAetherClientRequestState::Rejected;
    OnRequestChanged.Broadcast(*Request);
    return true;
}

int32 UAetherClientSubsystem::ExpireRequests(double NowSeconds)
{
    int32 Count = 0;
    for (TPair<uint32, FAetherClientRequestState>& Pair : Requests)
    {
        if (Pair.Value.IsExpired(NowSeconds))
        {
            Pair.Value.State = EAetherClientRequestState::TimedOut;
            OnRequestChanged.Broadcast(Pair.Value);
            ++Count;
        }
    }
    return Count;
}

bool UAetherClientSubsystem::GetRequest(uint32 RequestID, FAetherClientRequestState& OutRequest) const
{
    const FAetherClientRequestState* Request = Requests.Find(RequestID);
    if (!Request) return false;
    OutRequest = *Request;
    return true;
}

bool UAetherClientSubsystem::ApplyIdentity(const FAetherClientIdentitySnapshot& Identity, int64 ServerRevision, double ServerTimeSeconds)
{
    if (!Identity.HasAccount()) return false;
    if (ServerRevision < Snapshot.ServerRevision) return false;
    Snapshot.Identity = Identity;
    Snapshot.ServerRevision = ServerRevision;
    Snapshot.LastServerTimeSeconds = ServerTimeSeconds;
    if (Snapshot.ConnectionState == EAetherClientConnectionState::Connected
        || Snapshot.ConnectionState == EAetherClientConnectionState::Authenticating
        || Snapshot.ConnectionState == EAetherClientConnectionState::Reconnecting)
    {
        SetConnectionState(EAetherClientConnectionState::Authenticated);
    }
    return true;
}

void UAetherClientSubsystem::ClearIdentity()
{
    Snapshot.Identity.Reset();
    if (Snapshot.ConnectionState == EAetherClientConnectionState::Authenticated)
        SetConnectionState(EAetherClientConnectionState::Connected);
}
