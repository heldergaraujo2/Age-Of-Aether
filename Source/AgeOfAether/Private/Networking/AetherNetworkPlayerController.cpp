#include "Networking/AetherNetworkPlayerController.h"

#include "Accounts/AetherAccountSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "HAL/PlatformTime.h"
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
    Request.ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerSubmitRequest_Implementation(Request);
        return;
    }

    ServerSubmitRequest(Request);
}

void AAetherNetworkPlayerController::AuthenticateAccount(const FString& Username, const FString& CredentialProof)
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerAuthenticateAccount_Implementation(RequestId, Username, CredentialProof, ProtocolVersion);
        return;
    }

    ServerAuthenticateAccount(RequestId, Username, CredentialProof, ProtocolVersion);
}

void AAetherNetworkPlayerController::ReconnectAccount(
    const FAetherAccountId& AccountId,
    const FAetherSessionId& InSessionId,
    const FString& CredentialProof)
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerReconnectAccount_Implementation(RequestId, AccountId, InSessionId, CredentialProof, ProtocolVersion);
        return;
    }

    ServerReconnectAccount(RequestId, AccountId, InSessionId, CredentialProof, ProtocolVersion);
}

void AAetherNetworkPlayerController::LogoutAccount()
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerLogoutAccount_Implementation(RequestId, SessionId, ProtocolVersion);
        return;
    }

    ServerLogoutAccount(RequestId, SessionId, ProtocolVersion);
}

void AAetherNetworkPlayerController::SendSessionHeartbeat()
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
            : nullptr;

        const bool bAccepted = Sessions && bAccountAuthenticated
            && Sessions->Heartbeat(SessionId, ProtocolVersion, GetServerTimeSeconds());

        FAetherAuthenticationResponse Response;
        Response.Result = bAccepted
            ? EAetherAuthenticationResult::Accepted
            : EAetherAuthenticationResult::SessionNotFound;
        Response.AccountId = AuthenticatedAccountId;
        Response.SessionId = SessionId;
        ClientReceiveSessionHeartbeat(RequestId, Response);
        return;
    }

    ServerSessionHeartbeat(RequestId, SessionId, ProtocolVersion);
}

bool AAetherNetworkPlayerController::IsAccountAuthenticated() const
{
    return bAccountAuthenticated;
}

FAetherAccountId AAetherNetworkPlayerController::GetAuthenticatedAccountId() const
{
    return AuthenticatedAccountId;
}

FAetherSessionId AAetherNetworkPlayerController::GetSessionId() const
{
    return SessionId;
}

void AAetherNetworkPlayerController::ServerSubmitRequest_Implementation(const FAetherNetworkRequest& Request)
{
    FAetherNetworkResponse Response;
    Response.RequestId = Request.RequestId;
    Response.ProtocolVersion = FAetherProtocolVersion::Current();

    if (!ValidateRequest(Request))
    {
        Response.Result = Request.ProtocolVersion.IsCompatibleWith(Response.ProtocolVersion)
            ? EAetherNetworkResultCode::InvalidRequest
            : EAetherNetworkResultCode::InvalidProtocol;

        ClientReceiveResponse(Response);
        return;
    }

    const AAetherNetworkGameState* NetworkState = GetWorld()
        ? GetWorld()->GetGameState<AAetherNetworkGameState>()
        : nullptr;

    if (!NetworkState)
    {
        Response.Result = EAetherNetworkResultCode::ServerUnavailable;
        ClientReceiveResponse(Response);
        return;
    }

    LastProcessedRequestId = Request.RequestId;
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

void AAetherNetworkPlayerController::ServerAuthenticateAccount_Implementation(
    uint32 RequestId,
    const FString& Username,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!ValidateAccountRequestId(RequestId))
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        ClientReceiveAuthenticationResponse(RequestId, Response);
        return;
    }

    if (bAccountAuthenticated)
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::AlreadyOnline;
        Response.AccountId = AuthenticatedAccountId;
        Response.SessionId = SessionId;
        ClientReceiveAuthenticationResponse(RequestId, Response);
        return;
    }

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    FAetherAuthenticationResponse Response;
    if (!Sessions)
    {
        Response.Result = EAetherAuthenticationResult::AuthenticationUnavailable;
    }
    else
    {
        Response = Sessions->Authenticate(
            Username,
            CredentialProof,
            ProtocolVersion,
            GetServerTimeSeconds());

        if (Response.Result == EAetherAuthenticationResult::Accepted)
        {
            AuthenticatedAccountId = Response.AccountId;
            SessionId = Response.SessionId;
            bAccountAuthenticated = true;
        }
    }

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveAuthenticationResponse(RequestId, Response);
}

void AAetherNetworkPlayerController::ClientReceiveAuthenticationResponse_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    if (Response.Result == EAetherAuthenticationResult::Accepted)
    {
        ApplyAuthenticatedSession(Response);
    }

    OnAuthenticationResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerReconnectAccount_Implementation(
    uint32 RequestId,
    const FAetherAccountId& AccountId,
    const FAetherSessionId& InSessionId,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!ValidateAccountRequestId(RequestId))
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        ClientReceiveReconnectResponse(RequestId, Response);
        return;
    }

    if (bAccountAuthenticated)
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::AlreadyOnline;
        Response.AccountId = AuthenticatedAccountId;
        Response.SessionId = SessionId;
        ClientReceiveReconnectResponse(RequestId, Response);
        return;
    }

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    FAetherAuthenticationResponse Response;
    if (!Sessions)
    {
        Response.Result = EAetherAuthenticationResult::AuthenticationUnavailable;
    }
    else
    {
        Response = Sessions->Reconnect(
            AccountId,
            InSessionId,
            CredentialProof,
            ProtocolVersion,
            GetServerTimeSeconds());

        if (Response.Result == EAetherAuthenticationResult::Accepted)
        {
            AuthenticatedAccountId = Response.AccountId;
            SessionId = Response.SessionId;
            bAccountAuthenticated = true;
        }
    }

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveReconnectResponse(RequestId, Response);
}

void AAetherNetworkPlayerController::ClientReceiveReconnectResponse_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    if (Response.Result == EAetherAuthenticationResult::Accepted)
    {
        ApplyAuthenticatedSession(Response);
    }

    OnAuthenticationResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerLogoutAccount_Implementation(
    uint32 RequestId,
    const FAetherSessionId& InSessionId,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!ValidateAccountRequestId(RequestId))
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        ClientReceiveLogoutResponse(RequestId, Response);
        return;
    }

    FAetherAuthenticationResponse Response;
    Response.AccountId = AuthenticatedAccountId;
    Response.SessionId = InSessionId;

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    const bool bLoggedOut = Sessions
        && bAccountAuthenticated
        && InSessionId == SessionId
        && Sessions->Logout(InSessionId, ProtocolVersion, GetServerTimeSeconds());

    Response.Result = bLoggedOut
        ? EAetherAuthenticationResult::Accepted
        : EAetherAuthenticationResult::SessionNotFound;

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveLogoutResponse(RequestId, Response);

    if (bLoggedOut)
    {
        ClearAuthenticatedSession();
    }
}

void AAetherNetworkPlayerController::ClientReceiveLogoutResponse_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    if (Response.Result == EAetherAuthenticationResult::Accepted)
    {
        ClearAuthenticatedSession();
    }

    OnAuthenticationResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerSessionHeartbeat_Implementation(
    uint32 RequestId,
    const FAetherSessionId& InSessionId,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!ValidateAccountRequestId(RequestId))
    {
        return;
    }

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    FAetherAuthenticationResponse Response;
    Response.AccountId = AuthenticatedAccountId;
    Response.SessionId = InSessionId;
    Response.Result = Sessions
        && bAccountAuthenticated
        && InSessionId == SessionId
        && Sessions->Heartbeat(InSessionId, ProtocolVersion, GetServerTimeSeconds())
        ? EAetherAuthenticationResult::Accepted
        : EAetherAuthenticationResult::SessionNotFound;

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveSessionHeartbeat(RequestId, Response);
}

void AAetherNetworkPlayerController::ClientReceiveSessionHeartbeat_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    OnSessionHeartbeat.Broadcast(Response.Result == EAetherAuthenticationResult::Accepted);
}

bool AAetherNetworkPlayerController::ValidateRequest(const FAetherNetworkRequest& Request) const
{
    const FAetherProtocolVersion ServerProtocol = FAetherProtocolVersion::Current();

    if (!Request.ProtocolVersion.IsCompatibleWith(ServerProtocol))
    {
        return false;
    }

    if (Request.RequestId == 0 || Request.RequestId <= LastProcessedRequestId)
    {
        return false;
    }

    return true;
}

bool AAetherNetworkPlayerController::ValidateAccountRequestId(uint32 RequestId) const
{
    return RequestId != 0 && RequestId > LastProcessedAccountRequestId;
}

double AAetherNetworkPlayerController::GetServerTimeSeconds() const
{
    return FPlatformTime::Seconds();
}

void AAetherNetworkPlayerController::ApplyAuthenticatedSession(const FAetherAuthenticationResponse& Response)
{
    AuthenticatedAccountId = Response.AccountId;
    SessionId = Response.SessionId;
    bAccountAuthenticated = true;
}

void AAetherNetworkPlayerController::ClearAuthenticatedSession()
{
    AuthenticatedAccountId = FAetherAccountId();
    SessionId = FAetherSessionId();
    bAccountAuthenticated = false;
}
