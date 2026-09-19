#pragma once

#include "CoreMinimal.h"
#include "AetherAccountSessionTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherAccountStatus : uint8
{
    Active,
    Disabled,
    Locked
};

UENUM(BlueprintType)
enum class EAetherSessionState : uint8
{
    Authenticating,
    Authenticated,
    Active,
    Closing,
    Closed,
    TimedOut,
    Reconnected,
    Rejected
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAetherPermission : uint8
{
    None = 0,
    Player = 1 << 0,
    VIP = 1 << 1,
    Moderator = 1 << 2,
    GM = 1 << 3,
    Admin = 1 << 4,
    Owner = 1 << 5
};
ENUM_CLASS_FLAGS(EAetherPermission)

UENUM(BlueprintType)
enum class EAetherAuthenticationResult : uint8
{
    Accepted,
    InvalidProtocol,
    InvalidRequest,
    InvalidCredentials,
    AccountDisabled,
    AccountLocked,
    AlreadyOnline,
    SessionNotFound,
    SessionExpired,
    AuthenticationUnavailable
};

USTRUCT(BlueprintType)
struct FAetherAccountId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherAccountId NewId()
    {
        FAetherAccountId Id;
        Id.Value = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        return Id;
    }

    bool operator==(const FAetherAccountId& Other) const { return Value == Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FAetherAccountId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherSessionId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherSessionId NewId()
    {
        FAetherSessionId Id;
        Id.Value = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        return Id;
    }

    bool operator==(const FAetherSessionId& Other) const { return Value == Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FAetherSessionId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherAccountRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    FString Username;

    UPROPERTY(BlueprintReadOnly)
    EAetherAccountStatus Status = EAetherAccountStatus::Active;

    UPROPERTY(BlueprintReadOnly)
    int32 Permissions = static_cast<int32>(EAetherPermission::Player);
};

USTRUCT(BlueprintType)
struct FAetherSessionRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherSessionId SessionId;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    EAetherSessionState State = EAetherSessionState::Authenticating;

    UPROPERTY(BlueprintReadOnly)
    double CreatedAtSeconds = 0.0;

    UPROPERTY(BlueprintReadOnly)
    double LastHeartbeatAtSeconds = 0.0;
};

USTRUCT(BlueprintType)
struct FAetherAuthenticationResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherAuthenticationResult Result = EAetherAuthenticationResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    FAetherSessionId SessionId;

    UPROPERTY(BlueprintReadOnly)
    int32 Permissions = 0;

    UPROPERTY(BlueprintReadOnly)
    float HeartbeatTimeoutSeconds = 0.0f;
};
