#pragma once

#include "CoreMinimal.h"
#include "Security/AetherSecurityTypes.h"

class AGEOFAETHER_API FAetherSecurityService
{
public:
    bool Initialize(const FAetherSecurityConfig& InConfig);
    void Shutdown();

    EAetherSecurityResult AuthorizeRequest(
        uint32 ConnectionId,
        uint32 RequestId,
        EAetherSecurityAction Action,
        bool bAuthenticated,
        double NowSeconds);

    EAetherSecurityResult RecordInvalidRequest(
        uint32 ConnectionId,
        EAetherSecurityAction Action,
        uint32 RequestId,
        double NowSeconds);

    bool ValidateMovement(
        uint32 ConnectionId,
        const FVector& AuthoritativeLocation,
        double NowSeconds);

    void RecordSuspiciousEvent(
        uint32 ConnectionId,
        EAetherSecurityAction Action,
        uint32 RequestId,
        double NowSeconds,
        int32 Severity = 1);

    bool IsQuarantined(uint32 ConnectionId, double NowSeconds) const;
    int32 GetSuspicionScore(uint32 ConnectionId) const;
    const FAetherSecurityConnectionState* FindConnection(uint32 ConnectionId) const;
    const TArray<FAetherSecurityAuditEvent>& GetAuditEvents() const { return AuditEvents; }
    const FAetherSecurityConfig& GetConfig() const { return Config; }

private:
    void Refill(FAetherSecurityConnectionState& State, double NowSeconds) const;
    void AddAudit(
        uint32 ConnectionId,
        EAetherSecurityAction Action,
        EAetherSecurityResult Result,
        uint32 RequestId,
        double NowSeconds,
        int32 SuspicionScore);

    static constexpr int32 MaxAuditEvents = 512;

    FAetherSecurityConfig Config;
    TMap<uint32, FAetherSecurityConnectionState> Connections;
    TArray<FAetherSecurityAuditEvent> AuditEvents;
    bool bRunning = false;
};
