#include "Networking/AetherNetworkPlayerController.h"

#include "Networking/AetherNetworkGameState.h"

AAetherNetworkPlayerController::AAetherNetworkPlayerController()
{
    bReplicates = true;
}

void AAetherNetworkPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AAetherNetworkPlayerController::SendNetworkRequest(EAetherNetworkRequestType Type)
{
    FAetherNetworkRequest Request;
    Request.RequestId = NextRequestId++;
    Request.Type = Type;
    Request.ProtocolVersion.Major = 1;
    Request.ProtocolVersion.Minor = 0;

    if (HasAuthority())
    {
        ServerSubmitRequest_Implementation(Request);
        return;
    }

    ServerSubmitRequest(Request);
}

void AAetherNetworkPlayerController::ServerSubmitRequest_Implementation(const FAetherNetworkRequest& Request)
{
    FAetherNetworkResponse Response;
    Response.RequestId = Request.RequestId;
    Response.ProtocolVersion.Major = 1;
    Response.ProtocolVersion.Minor = 0;

    if (!ValidateRequest(Request))
    {
        Response.Result = Request.ProtocolVersion.IsCompatibleWith(Response.ProtocolVersion)
            ? EAetherNetworkResultCode::InvalidRequest
            : EAetherNetworkResultCode::InvalidProtocol;

        ClientReceiveResponse(Response);
        return;
    }

    const AAetherNetworkGameState* NetworkState = GetWorld() ? GetWorld()->GetGameState<AAetherNetworkGameState>() : nullptr;
    if (!NetworkState)
    {
        Response.Result = EAetherNetworkResultCode::ServerUnavailable;
        ClientReceiveResponse(Response);
        return;
    }

    Response.Result = EAetherNetworkResultCode::Accepted;
    Response.AuthoritativeStateRevision = NetworkState->GetAuthoritativeStateRevision();

    if (Request.Type == EAetherNetworkRequestType::Heartbeat)
    {
        if (AAetherNetworkGameState* MutableState = GetWorld()->GetGameState<AAetherNetworkGameState>())
        {
            MutableState->AdvanceAuthoritativeState();
            Response.AuthoritativeStateRevision = MutableState->GetAuthoritativeStateRevision();
        }
    }

    ClientReceiveResponse(Response);
}

void AAetherNetworkPlayerController::ClientReceiveResponse_Implementation(const FAetherNetworkResponse& Response)
{
    OnNetworkResponse.Broadcast(Response);
}

bool AAetherNetworkPlayerController::ValidateRequest(const FAetherNetworkRequest& Request) const
{
    FAetherProtocolVersion ServerProtocol;
    ServerProtocol.Major = 1;
    ServerProtocol.Minor = 0;

    if (!Request.ProtocolVersion.IsCompatibleWith(ServerProtocol))
    {
        return false;
    }

    return Request.RequestId != 0;
}
