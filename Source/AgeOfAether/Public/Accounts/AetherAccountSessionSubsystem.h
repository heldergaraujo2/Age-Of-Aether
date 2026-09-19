#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "Accounts/AetherAccountSessionService.h"

#include "AetherAccountSessionSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherAccountSessionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void SetCredentialVerifier(FAetherAccountSessionService::FCredentialVerifier InVerifier);

    bool RegisterAccount(const FAetherAccountRecord& Account);
    bool UpdateAccountStatus(const FAetherAccountId& AccountId, EAetherAccountStatus Status);

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

    int32 NumAccounts() const;
    int32 NumSessions() const;

private:
    bool Tick(float DeltaTime);

    FAetherAccountSessionService SessionService;
    FTSTicker::FDelegateHandle TickerHandle;
};
