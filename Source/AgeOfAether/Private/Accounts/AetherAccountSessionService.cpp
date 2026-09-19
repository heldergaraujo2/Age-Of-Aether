#include "Accounts/AetherAccountSessionService.h"

FAetherAccountSessionService::FAetherAccountSessionService(double InHeartbeatTimeoutSeconds)
    : HeartbeatTimeoutSeconds(FMath::Max(1.0, InHeartbeatTimeoutSeconds))
{
}

void FAetherAccountSessionService::SetCredentialVerifier(FCredentialVerifier InVerifier)
{
    CredentialVerifier = MoveTemp(InVerifier);
}

bool FAetherAccountSessionService::RegisterAccount(const FAetherAccountRecord& Account)
{
    if (!Account.AccountId.IsValid())
    {
        return false;
    }

    const FString NormalizedUsername = NormalizeUsername(Account.Username);
    if (NormalizedUsername.IsEmpty() || AccountsByUsername.Contains(NormalizedUsername))
    {
        return false;
    }

    FAetherAccountRecord StoredAccount = Account;
    StoredAccount.Username = NormalizedUsername;
    AccountsByUsername.Add(NormalizedUsername, StoredAccount);
    AccountUsernameById.Add(StoredAccount.AccountId, NormalizedUsername);
    return true;
}

bool FAetherAccountSessionService::UpdateAccountStatus(const FAetherAccountId& AccountId, EAetherAccountStatus Status)
{
    const FString* Username = AccountUsernameById.Find(AccountId);
    if (!Username)
    {
        return false;
    }

    FAetherAccountRecord* Account = AccountsByUsername.Find(*Username);
    if (!Account)
    {
        return false;
    }

    Account->Status = Status;
    if (Status != EAetherAccountStatus::Active)
    {
        if (const FAetherSessionId* ActiveSessionId = ActiveSessionByAccount.Find(AccountId))
        {
            if (FAetherSessionRecord* Session = Sessions.Find(*ActiveSessionId))
            {
                CloseSession(*Session, EAetherSessionState::Rejected, Session->LastHeartbeatAtSeconds);
            }
        }
    }

    return true;
}

bool FAetherAccountSessionService::FindAccountByUsername(const FString& Username, FAetherAccountRecord& OutAccount) const
{
    const FAetherAccountRecord* Account = AccountsByUsername.Find(NormalizeUsername(Username));
    if (!Account)
    {
        return false;
    }

    OutAccount = *Account;
    return true;
}

bool FAetherAccountSessionService::FindSession(const FAetherSessionId& SessionId, FAetherSessionRecord& OutSession) const
{
    const FAetherSessionRecord* Session = Sessions.Find(SessionId);
    if (!Session)
    {
        return false;
    }

    OutSession = *Session;
    return true;
}

FAetherAuthenticationResponse FAetherAccountSessionService::Authenticate(
    const FString& Username,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    FAetherAuthenticationResponse Response;
    Response.HeartbeatTimeoutSeconds = static_cast<float>(HeartbeatTimeoutSeconds);

    if (!IsProtocolCompatible(ProtocolVersion))
    {
        Response.Result = EAetherAuthenticationResult::InvalidProtocol;
        return Response;
    }

    const FString NormalizedUsername = NormalizeUsername(Username);
    if (NormalizedUsername.IsEmpty() || CredentialProof.IsEmpty())
    {
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        return Response;
    }

    const FAetherAccountRecord* Account = AccountsByUsername.Find(NormalizedUsername);
    if (!Account || !CredentialVerifier || !CredentialVerifier(NormalizedUsername, CredentialProof))
    {
        Response.Result = EAetherAuthenticationResult::InvalidCredentials;
        return Response;
    }

    if (Account->Status == EAetherAccountStatus::Disabled)
    {
        Response.Result = EAetherAuthenticationResult::AccountDisabled;
        return Response;
    }

    if (Account->Status == EAetherAccountStatus::Locked)
    {
        Response.Result = EAetherAuthenticationResult::AccountLocked;
        return Response;
    }

    if (HasOnlineSessionForAccount(Account->AccountId))
    {
        Response.Result = EAetherAuthenticationResult::AlreadyOnline;
        return Response;
    }

    FAetherSessionRecord Session;
    Session.SessionId = FAetherSessionId::NewId();
    Session.AccountId = Account->AccountId;
    Session.State = EAetherSessionState::Active;
    Session.CreatedAtSeconds = NowSeconds;
    Session.LastHeartbeatAtSeconds = NowSeconds;

    Sessions.Add(Session.SessionId, Session);
    ActiveSessionByAccount.Add(Account->AccountId, Session.SessionId);

    Response.Result = EAetherAuthenticationResult::Accepted;
    Response.AccountId = Account->AccountId;
    Response.SessionId = Session.SessionId;
    Response.Permissions = Account->Permissions;
    return Response;
}

FAetherAuthenticationResponse FAetherAccountSessionService::Reconnect(
    const FAetherAccountId& AccountId,
    const FAetherSessionId& SessionId,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    FAetherAuthenticationResponse Response;
    Response.HeartbeatTimeoutSeconds = static_cast<float>(HeartbeatTimeoutSeconds);

    if (!IsProtocolCompatible(ProtocolVersion))
    {
        Response.Result = EAetherAuthenticationResult::InvalidProtocol;
        return Response;
    }

    FAetherSessionRecord* Session = Sessions.Find(SessionId);
    if (!Session || Session->AccountId != AccountId)
    {
        Response.Result = EAetherAuthenticationResult::SessionNotFound;
        return Response;
    }

    if (!IsSessionActive(*Session))
    {
        Response.Result = Session->State == EAetherSessionState::TimedOut
            ? EAetherAuthenticationResult::SessionExpired
            : EAetherAuthenticationResult::SessionNotFound;
        return Response;
    }

    const FString* Username = AccountUsernameById.Find(AccountId);
    const FAetherAccountRecord* Account = Username ? AccountsByUsername.Find(*Username) : nullptr;
    if (!Account || !CredentialVerifier || CredentialProof.IsEmpty() || !CredentialVerifier(Account->Username, CredentialProof))
    {
        Response.Result = EAetherAuthenticationResult::InvalidCredentials;
        return Response;
    }

    if (Account->Status == EAetherAccountStatus::Disabled)
    {
        Response.Result = EAetherAuthenticationResult::AccountDisabled;
        return Response;
    }

    if (Account->Status == EAetherAccountStatus::Locked)
    {
        Response.Result = EAetherAuthenticationResult::AccountLocked;
        return Response;
    }

    Session->State = EAetherSessionState::Active;
    Session->LastHeartbeatAtSeconds = NowSeconds;

    Response.Result = EAetherAuthenticationResult::Accepted;
    Response.AccountId = Account->AccountId;
    Response.SessionId = Session->SessionId;
    Response.Permissions = Account->Permissions;
    return Response;
}

bool FAetherAccountSessionService::Heartbeat(
    const FAetherSessionId& SessionId,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    if (!IsProtocolCompatible(ProtocolVersion))
    {
        return false;
    }

    FAetherSessionRecord* Session = Sessions.Find(SessionId);
    if (!Session || !IsSessionActive(*Session))
    {
        return false;
    }

    Session->State = EAetherSessionState::Active;
    Session->LastHeartbeatAtSeconds = NowSeconds;
    return true;
}

bool FAetherAccountSessionService::Logout(
    const FAetherSessionId& SessionId,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    if (!IsProtocolCompatible(ProtocolVersion))
    {
        return false;
    }

    FAetherSessionRecord* Session = Sessions.Find(SessionId);
    if (!Session || !IsSessionActive(*Session))
    {
        return false;
    }

    CloseSession(*Session, EAetherSessionState::Closed, NowSeconds);
    return true;
}

int32 FAetherAccountSessionService::ExpireTimedOutSessions(double NowSeconds)
{
    int32 ExpiredCount = 0;

    for (TPair<FAetherSessionId, FAetherSessionRecord>& Pair : Sessions)
    {
        FAetherSessionRecord& Session = Pair.Value;
        if (IsSessionActive(Session) && NowSeconds - Session.LastHeartbeatAtSeconds > HeartbeatTimeoutSeconds)
        {
            CloseSession(Session, EAetherSessionState::TimedOut, NowSeconds);
            ++ExpiredCount;
        }
    }

    return ExpiredCount;
}

int32 FAetherAccountSessionService::NumAccounts() const
{
    return AccountsByUsername.Num();
}

int32 FAetherAccountSessionService::NumSessions() const
{
    return Sessions.Num();
}

FString FAetherAccountSessionService::NormalizeUsername(const FString& Username)
{
    return Username.TrimStartAndEnd().ToLower();
}

bool FAetherAccountSessionService::IsProtocolCompatible(const FAetherProtocolVersion& ProtocolVersion)
{
    return ProtocolVersion.IsCompatibleWith(FAetherProtocolVersion::Current());
}

bool FAetherAccountSessionService::IsSessionActive(const FAetherSessionRecord& Session) const
{
    return Session.State == EAetherSessionState::Authenticated || Session.State == EAetherSessionState::Active;
}

bool FAetherAccountSessionService::HasOnlineSessionForAccount(const FAetherAccountId& AccountId) const
{
    const FAetherSessionId* SessionId = ActiveSessionByAccount.Find(AccountId);
    if (!SessionId)
    {
        return false;
    }

    const FAetherSessionRecord* Session = Sessions.Find(*SessionId);
    return Session && IsSessionActive(*Session);
}

void FAetherAccountSessionService::CloseSession(
    FAetherSessionRecord& Session,
    EAetherSessionState FinalState,
    double NowSeconds)
{
    Session.State = FinalState;
    Session.LastHeartbeatAtSeconds = NowSeconds;

    if (const FAetherSessionId* ActiveSessionId = ActiveSessionByAccount.Find(Session.AccountId))
    {
        if (*ActiveSessionId == Session.SessionId)
        {
            ActiveSessionByAccount.Remove(Session.AccountId);
        }
    }
}
