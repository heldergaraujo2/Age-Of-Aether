#include "Security/AetherSecurityService.h"

#include "Math/UnrealMathUtility.h"

bool FAetherSecurityService::Initialize(const FAetherSecurityConfig& InConfig)
{
    if (!InConfig.IsValid())
    {
        return false;
    }

    Config = InConfig;
    Connections.Reset();
    AuditEvents.Reset();
    bRunning = true;
    return true;
}

void FAetherSecurityService::Shutdown()
{
    Connections.Reset();
    AuditEvents.Reset();
    bRunning = false;
}

void FAetherSecurityService::Refill(FAetherSecurityConnectionState& State, double NowSeconds) const
{
    const double Elapsed = FMath::Max(0.0, NowSeconds - State.LastRefillAt);
    if (Elapsed <= 0.0)
    {
        return;
    }

    const int32 RefillAmount = FMath::FloorToInt(Elapsed * Config.RequestsPerSecond);
    if (RefillAmount > 0)
    {
        State.RequestBudget = FMath::Min(Config.BurstCapacity, State.RequestBudget + RefillAmount);
        State.LastRefillAt += static_cast<double>(RefillAmount) / Config.RequestsPerSecond;
    }
}

EAetherSecurityResult FAetherSecurityService::AuthorizeRequest(
    uint32 ConnectionId,
    uint32 RequestId,
    EAetherSecurityAction Action,
    bool bAuthenticated,
    double NowSeconds)
{
    if (!bRunning || ConnectionId == 0 || RequestId == 0)
    {
        return EAetherSecurityResult::InvalidRequest;
    }

    FAetherSecurityConnectionState& State = Connections.FindOrAdd(ConnectionId);
    if (State.ConnectionId == 0)
    {
        State.ConnectionId = ConnectionId;
        State.RequestBudget = Config.BurstCapacity;
        State.LastRefillAt = NowSeconds;
    }

    Refill(State, NowSeconds);

    if (State.QuarantinedUntil > NowSeconds)
    {
        AddAudit(ConnectionId, Action, EAetherSecurityResult::Quarantined, RequestId, NowSeconds, State.SuspicionScore);
        return EAetherSecurityResult::Quarantined;
    }

    if (RequestId <= State.LastRequestId)
    {
        ++State.InvalidRequestCount;
        ++State.SuspicionScore;
        AddAudit(ConnectionId, Action, EAetherSecurityResult::ReplayRejected, RequestId, NowSeconds, State.SuspicionScore);
        return EAetherSecurityResult::ReplayRejected;
    }

    if (State.RequestBudget <= 0)
    {
        ++State.SuspicionScore;
        AddAudit(ConnectionId, Action, EAetherSecurityResult::RateLimited, RequestId, NowSeconds, State.SuspicionScore);
        return EAetherSecurityResult::RateLimited;
    }

    --State.RequestBudget;
    State.LastRequestId = RequestId;

    if (!bAuthenticated && Action != EAetherSecurityAction::Authentication
        && Action != EAetherSecurityAction::Session)
    {
        AddAudit(ConnectionId, Action, EAetherSecurityResult::NotAuthenticated, RequestId, NowSeconds, State.SuspicionScore);
        return EAetherSecurityResult::NotAuthenticated;
    }

    AddAudit(ConnectionId, Action, EAetherSecurityResult::Accepted, RequestId, NowSeconds, State.SuspicionScore);
    return EAetherSecurityResult::Accepted;
}

EAetherSecurityResult FAetherSecurityService::RecordInvalidRequest(
    uint32 ConnectionId,
    EAetherSecurityAction Action,
    uint32 RequestId,
    double NowSeconds)
{
    FAetherSecurityConnectionState& State = Connections.FindOrAdd(ConnectionId);
    State.ConnectionId = ConnectionId;
    ++State.InvalidRequestCount;
    ++State.SuspicionScore;

    if (State.InvalidRequestCount >= Config.MaxInvalidRequestsBeforeQuarantine)
    {
        State.QuarantinedUntil = NowSeconds + Config.QuarantineSeconds;
        AddAudit(ConnectionId, Action, EAetherSecurityResult::Quarantined, RequestId, NowSeconds, State.SuspicionScore);
        return EAetherSecurityResult::Quarantined;
    }

    AddAudit(ConnectionId, Action, EAetherSecurityResult::Suspicious, RequestId, NowSeconds, State.SuspicionScore);
    return EAetherSecurityResult::Suspicious;
}

bool FAetherSecurityService::ValidateMovement(
    uint32 ConnectionId,
    const FVector& AuthoritativeLocation,
    double NowSeconds)
{
    FAetherSecurityConnectionState* State = Connections.Find(ConnectionId);
    if (!State)
    {
        return false;
    }

    if (State->LastMovementTime <= 0.0)
    {
        State->LastMovementTime = NowSeconds;
        State->LastAuthoritativeLocation = AuthoritativeLocation;
        return true;
    }

    const double DeltaTime = NowSeconds - State->LastMovementTime;
    if (DeltaTime <= 0.0)
    {
        return false;
    }

    const double AllowedDistance =
        Config.MaxMovementSpeedUnitsPerSecond * (DeltaTime + Config.MovementGraceSeconds);

    const double DistanceSquared =
        FVector::DistSquared(State->LastAuthoritativeLocation, AuthoritativeLocation);

    if (DistanceSquared > FMath::Square(AllowedDistance))
    {
        ++State->SuspicionScore;
        AddAudit(
            ConnectionId,
            EAetherSecurityAction::World,
            EAetherSecurityResult::Suspicious,
            0,
            NowSeconds,
            State->SuspicionScore);
        return false;
    }

    State->LastMovementTime = NowSeconds;
    State->LastAuthoritativeLocation = AuthoritativeLocation;
    return true;
}

void FAetherSecurityService::RecordSuspiciousEvent(
    uint32 ConnectionId,
    EAetherSecurityAction Action,
    uint32 RequestId,
    double NowSeconds,
    int32 Severity)
{
    FAetherSecurityConnectionState& State = Connections.FindOrAdd(ConnectionId);
    State.ConnectionId = ConnectionId;
    State.SuspicionScore += FMath::Max(1, Severity);
    AddAudit(ConnectionId, Action, EAetherSecurityResult::Suspicious, RequestId, NowSeconds, State.SuspicionScore);
}

bool FAetherSecurityService::IsQuarantined(uint32 ConnectionId, double NowSeconds) const
{
    const FAetherSecurityConnectionState* State = Connections.Find(ConnectionId);
    return State && State->QuarantinedUntil > NowSeconds;
}

int32 FAetherSecurityService::GetSuspicionScore(uint32 ConnectionId) const
{
    const FAetherSecurityConnectionState* State = Connections.Find(ConnectionId);
    return State ? State->SuspicionScore : 0;
}

const FAetherSecurityConnectionState* FAetherSecurityService::FindConnection(uint32 ConnectionId) const
{
    return Connections.Find(ConnectionId);
}

void FAetherSecurityService::AddAudit(
    uint32 ConnectionId,
    EAetherSecurityAction Action,
    EAetherSecurityResult Result,
    uint32 RequestId,
    double NowSeconds,
    int32 SuspicionScore)
{
    FAetherSecurityAuditEvent Event;
    Event.ConnectionId = ConnectionId;
    Event.Action = Action;
    Event.Result = Result;
    Event.RequestId = RequestId;
    Event.ServerTime = NowSeconds;
    Event.SuspicionScore = SuspicionScore;

    if (AuditEvents.Num() >= MaxAuditEvents)
    {
        AuditEvents.RemoveAt(0, 1, EAllowShrinking::No);
    }

    AuditEvents.Add(Event);
}
