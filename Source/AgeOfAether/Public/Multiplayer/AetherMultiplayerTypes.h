#pragma once
#include "CoreMinimal.h"
#include "AetherMultiplayerTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherAdmissionResult : uint8 { Accepted, ServerFull, AlreadyConnected, InvalidConnection };

UENUM(BlueprintType)
enum class EAetherAuthorityResult : uint8 { Accepted, InvalidConnection, RateLimited, NotAuthenticated, ServerShuttingDown };

USTRUCT(BlueprintType)
struct FAetherMultiplayerConfig
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxPlayers = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequestsPerSecond = 30;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 BurstCapacity = 60;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double HeartbeatTimeoutSeconds = 30.0;
    bool IsValid() const { return MaxPlayers > 0 && RequestsPerSecond > 0 && BurstCapacity >= RequestsPerSecond && HeartbeatTimeoutSeconds >= 5.0; }
};

USTRUCT(BlueprintType)
struct FAetherMultiplayerConnection
{
    GENERATED_BODY()
    UPROPERTY() uint32 ConnectionId = 0;
    UPROPERTY(BlueprintReadOnly) bool bAuthenticated = false;
    UPROPERTY(BlueprintReadOnly) double ConnectedAt = 0.0;
    UPROPERTY(BlueprintReadOnly) double LastHeartbeatAt = 0.0;
    UPROPERTY(BlueprintReadOnly) int32 RequestBudget = 0;
    UPROPERTY(BlueprintReadOnly) double LastRequestRefillAt = 0.0;
};