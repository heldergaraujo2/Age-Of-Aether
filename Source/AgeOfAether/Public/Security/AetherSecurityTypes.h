#pragma once

#include "CoreMinimal.h"
#include "AetherSecurityTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherSecurityAction : uint8
{
    Authentication,
    Session,
    Character,
    Inventory,
    Progression,
    Combat,
    World,
    Quest,
    Social,
    Economy,
    Generic
};

UENUM(BlueprintType)
enum class EAetherSecurityResult : uint8
{
    Accepted,
    InvalidRequest,
    RateLimited,
    ReplayRejected,
    Suspicious,
    Quarantined,
    NotAuthenticated
};

USTRUCT(BlueprintType)
struct FAetherSecurityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequestsPerSecond = 30;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BurstCapacity = 60;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxInvalidRequestsBeforeQuarantine = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    double QuarantineSeconds = 10.0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    double MaxMovementSpeedUnitsPerSecond = 1200.0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    double MovementGraceSeconds = 0.25;

    bool IsValid() const
    {
        return RequestsPerSecond > 0
            && BurstCapacity >= RequestsPerSecond
            && MaxInvalidRequestsBeforeQuarantine > 0
            && QuarantineSeconds >= 1.0
            && MaxMovementSpeedUnitsPerSecond > 0.0
            && MovementGraceSeconds >= 0.0;
    }
};

USTRUCT(BlueprintType)
struct FAetherSecurityConnectionState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    uint32 ConnectionId = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 RequestBudget = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 InvalidRequestCount = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 SuspicionScore = 0;

    UPROPERTY(BlueprintReadOnly)
    double LastRefillAt = 0.0;

    UPROPERTY(BlueprintReadOnly)
    double QuarantinedUntil = 0.0;

    uint32 LastRequestId = 0;
    double LastMovementTime = 0.0;
    FVector LastAuthoritativeLocation = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAetherSecurityAuditEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    uint32 ConnectionId = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherSecurityAction Action = EAetherSecurityAction::Generic;

    UPROPERTY(BlueprintReadOnly)
    EAetherSecurityResult Result = EAetherSecurityResult::Accepted;

    UPROPERTY(BlueprintReadOnly)
    uint32 RequestId = 0;

    UPROPERTY(BlueprintReadOnly)
    double ServerTime = 0.0;

    UPROPERTY(BlueprintReadOnly)
    int32 SuspicionScore = 0;
};
