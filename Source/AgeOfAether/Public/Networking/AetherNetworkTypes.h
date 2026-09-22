#pragma once

#include "CoreMinimal.h"
#include "AetherNetworkTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherNetworkRequestType : uint8
{
    Heartbeat,
    QueryAuthoritativeState
};

UENUM(BlueprintType)
enum class EAetherNetworkResultCode : uint8
{
    Accepted,
    InvalidProtocol,
    InvalidRequest,
    ServerUnavailable
};

USTRUCT(BlueprintType)
struct FAetherProtocolVersion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    uint16 Major = 1;

    UPROPERTY(EditAnywhere)
    uint16 Minor = 0;

    bool IsCompatibleWith(const FAetherProtocolVersion& Other) const
    {
        return Major == Other.Major;
    }

    static FAetherProtocolVersion Current()
    {
        FAetherProtocolVersion Version;
        Version.Major = 1;
        Version.Minor = 0;
        return Version;
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("%u.%u"), Major, Minor);
    }
};

USTRUCT(BlueprintType)
struct FAetherNetworkRequest
{
    GENERATED_BODY()

    UPROPERTY()
    uint32 RequestId = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherNetworkRequestType Type = EAetherNetworkRequestType::Heartbeat;

    UPROPERTY(BlueprintReadOnly)
    FAetherProtocolVersion ProtocolVersion;
};

USTRUCT(BlueprintType)
struct FAetherNetworkResponse
{
    GENERATED_BODY()

    UPROPERTY()
    uint32 RequestId = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherNetworkResultCode Result = EAetherNetworkResultCode::ServerUnavailable;

    UPROPERTY(BlueprintReadOnly)
    FAetherProtocolVersion ProtocolVersion;

    UPROPERTY()
    uint64 AuthoritativeStateRevision = 0;
};
