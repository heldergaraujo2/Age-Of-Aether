#include "Accounts/AetherAccountSessionSubsystem.h"

#include "HAL/PlatformTime.h"

void UAetherAccountSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateUObject(this, &UAetherAccountSessionSubsystem::Tick),
        1.0f);
}

void UAetherAccountSessionSubsystem::Deinitialize()
{
    if (TickerHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
        TickerHandle.Reset();
    }

    Super::Deinitialize();
}

void UAetherAccountSessionSubsystem::SetCredentialVerifier(FAetherAccountSessionService::FCredentialVerifier InVerifier)
{
    SessionService.SetCredentialVerifier(MoveTemp(InVerifier));
}

bool UAetherAccountSessionSubsystem::RegisterAccount(const FAetherAccountRecord& Account)
{
    return SessionService.RegisterAccount(Account);
}

bool UAetherAccountSessionSubsystem::UpdateAccountStatus(const FAetherAccountId& AccountId, EAetherAccountStatus Status)
{
    return SessionService.UpdateAccountStatus(AccountId, Status);
}

FAetherAuthenticationResponse UAetherAccountSessionSubsystem::Authenticate(
    const FString& Username,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    return SessionService.Authenticate(Username, CredentialProof, ProtocolVersion, NowSeconds);
}

FAetherAuthenticationResponse UAetherAccountSessionSubsystem::Reconnect(
    const FAetherAccountId& AccountId,
    const FAetherSessionId& SessionId,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    return SessionService.Reconnect(AccountId, SessionId, CredentialProof, ProtocolVersion, NowSeconds);
}

bool UAetherAccountSessionSubsystem::Heartbeat(
    const FAetherSessionId& SessionId,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    return SessionService.Heartbeat(SessionId, ProtocolVersion, NowSeconds);
}

bool UAetherAccountSessionSubsystem::Logout(
    const FAetherSessionId& SessionId,
    const FAetherProtocolVersion& ProtocolVersion,
    double NowSeconds)
{
    return SessionService.Logout(SessionId, ProtocolVersion, NowSeconds);
}

int32 UAetherAccountSessionSubsystem::NumAccounts() const
{
    return SessionService.NumAccounts();
}

int32 UAetherAccountSessionSubsystem::NumSessions() const
{
    return SessionService.NumSessions();
}

bool UAetherAccountSessionSubsystem::Tick(float DeltaTime)
{
    SessionService.ExpireTimedOutSessions(FPlatformTime::Seconds());
    return true;
}
