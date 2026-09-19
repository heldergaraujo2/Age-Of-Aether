#pragma once

#include "CoreMinimal.h"
#include "Accounts/AetherAccountSessionTypes.h"
#include "Networking/AetherNetworkTypes.h"

class FAetherAccountSessionService
{
public:
    using FCredentialVerifier = TFunction<bool(const FString& Username, const FString& CredentialProof)>;

    explicit FAetherAccountSessionService(double InHeartbeatTimeoutSeconds = 30.0);

    void SetCredentialVerifier(FCredentialVerifier InVerifier);

    bool RegisterAccount(const FAetherAccountRecord& Account);
    bool UpdateAccountStatus(const FAetherAccountId& AccountId, EAetherAccountStatus Status);
    bool FindAccountByUsername(const FString& Username, FAetherAccountRecord& OutAccount) const;
    bool FindSession(const FAetherSessionId& SessionId, FAetherSessionRecord& OutSession) const;

    FAetherAuthenticationResponse Authenticate(
        const FString& Username,
        const FString& CredentialProof,
        const FAetherProtocolVersion& ProtocolVersion,
        double NowSeconds);

    FAetherAuthenticationResponse Reconnect(
        const FAetherAccountId& AccountId,
        const FAetherSessionId& SessionId,
        const FString& CredentialProof,
        const FAetherProtocolVersion& ProtocolVersion,
        double NowSeconds);

    bool Heartbeat(
        const FAetherSessionId& SessionId,
        const FAetherProtocolVersion& ProtocolVersion,
        double NowSeconds);

    bool Logout(
        const FAetherSessionId& SessionId,
        const FAetherProtocolVersion& ProtocolVersion,
        double NowSeconds);

    int32 ExpireTimedOutSessions(double NowSeconds);
    int32 NumAccounts() const;
    int32 NumSessions() const;

private:
    static FString NormalizeUsername(const FString& Username);
    static bool IsProtocolCompatible(const FAetherProtocolVersion& ProtocolVersion);
    bool IsSessionActive(const FAetherSessionRecord& Session) const;
    bool HasOnlineSessionForAccount(const FAetherAccountId& AccountId) const;
    void CloseSession(FAetherSessionRecord& Session, EAetherSessionState FinalState, double NowSeconds);

    double HeartbeatTimeoutSeconds;
    FCredentialVerifier CredentialVerifier;
    TMap<FString, FAetherAccountRecord> AccountsByUsername;
    TMap<FAetherAccountId, FString> AccountUsernameById;
    TMap<FAetherSessionId, FAetherSessionRecord> Sessions;
    TMap<FAetherAccountId, FAetherSessionId> ActiveSessionByAccount;
};
