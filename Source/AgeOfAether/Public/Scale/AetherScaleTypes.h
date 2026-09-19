#pragma once
#include "CoreMinimal.h"
#include "AetherScaleTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherServerMode : uint8 { DedicatedServer, ListenServer, Standalone };

UENUM(BlueprintType)
enum class EAetherServerLifecycle : uint8 { Starting, Accepting, Draining, Stopped };

UENUM(BlueprintType)
enum class EAetherScaleResult : uint8
{
    Accepted, InvalidConfiguration, ServerDraining, ServerFull, ServerUnhealthy,
    InvalidNode, InvalidTransfer, TransferExpired
};

USTRUCT(BlueprintType)
struct FAetherScaleConfig
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxPlayers = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxConnectionsPerShard = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float TargetServerTickRate = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MaxServerTickRate = 60.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float SoftLoadThreshold = 0.75f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float HardLoadThreshold = 0.90f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double TransferTimeoutSeconds = 10.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double NodeHeartbeatTimeoutSeconds = 15.0;
    bool IsValid() const
    {
        return MaxPlayers > 0 && MaxConnectionsPerShard > 0 &&
            MaxConnectionsPerShard <= MaxPlayers && TargetServerTickRate > 0.0f &&
            MaxServerTickRate >= TargetServerTickRate && SoftLoadThreshold > 0.0f &&
            SoftLoadThreshold < 1.0f && HardLoadThreshold >= SoftLoadThreshold &&
            HardLoadThreshold <= 1.0f && TransferTimeoutSeconds >= 1.0 &&
            NodeHeartbeatTimeoutSeconds >= 5.0;
    }
};

USTRUCT(BlueprintType)
struct FAetherServerNodeId
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Value;
    bool IsValid() const { return !Value.TrimStartAndEnd().IsEmpty(); }
    bool operator==(const FAetherServerNodeId& Other) const { return Value == Other.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FAetherServerNodeId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherServerNodeDescriptor
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherServerNodeId NodeId;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Region;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ShardId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxPlayers = 1000;
    UPROPERTY(BlueprintReadOnly) int32 CurrentPlayers = 0;
    UPROPERTY(BlueprintReadOnly) EAetherServerMode Mode = EAetherServerMode::DedicatedServer;
    UPROPERTY(BlueprintReadOnly) EAetherServerLifecycle Lifecycle = EAetherServerLifecycle::Starting;
    UPROPERTY(BlueprintReadOnly) bool bHealthy = true;
    UPROPERTY(BlueprintReadOnly) double LastHeartbeatAt = 0.0;
};

USTRUCT(BlueprintType)
struct FAetherServerLoadSnapshot
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) int32 ActiveConnections = 0;
    UPROPERTY(BlueprintReadOnly) int32 AuthenticatedPlayers = 0;
    UPROPERTY(BlueprintReadOnly) int32 MaxPlayers = 0;
    UPROPERTY(BlueprintReadOnly) double ServerTickMilliseconds = 0.0;
    UPROPERTY(BlueprintReadOnly) double ReplicationMilliseconds = 0.0;
    UPROPERTY(BlueprintReadOnly) double NetworkMegabitsPerSecond = 0.0;
    float ConnectionLoad() const
    {
        return MaxPlayers > 0 ? static_cast<float>(ActiveConnections) / static_cast<float>(MaxPlayers) : 1.0f;
    }
    bool IsValid() const
    {
        return ActiveConnections >= 0 && AuthenticatedPlayers >= 0 && MaxPlayers > 0 &&
            ActiveConnections <= MaxPlayers && AuthenticatedPlayers <= ActiveConnections &&
            ServerTickMilliseconds >= 0.0 && ReplicationMilliseconds >= 0.0 &&
            NetworkMegabitsPerSecond >= 0.0;
    }
};

USTRUCT(BlueprintType)
struct FAetherServerTransferRequest
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) uint64 TransferId = 0;
    UPROPERTY(BlueprintReadOnly) FString AccountId;
    UPROPERTY(BlueprintReadOnly) FString CharacterId;
    UPROPERTY(BlueprintReadOnly) FAetherServerNodeId SourceNode;
    UPROPERTY(BlueprintReadOnly) FAetherServerNodeId DestinationNode;
    UPROPERTY(BlueprintReadOnly) FString TargetZoneId;
    UPROPERTY(BlueprintReadOnly) double CreatedAt = 0.0;
    UPROPERTY(BlueprintReadOnly) double ExpiresAt = 0.0;
};
