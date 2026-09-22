#pragma once
#include "CoreMinimal.h"
#include "AetherClientTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherClientConnectionState : uint8
{
    Offline,
    Connecting,
    Connected,
    Authenticating,
    Authenticated,
    Reconnecting,
    Disconnected,
    Error
};

UENUM(BlueprintType)
enum class EAetherClientScreen : uint8
{
    None,
    Login,
    CharacterSelect,
    LoadingWorld,
    Gameplay
};

UENUM(BlueprintType)
enum class EAetherClientRequestState : uint8
{
    Pending,
    Accepted,
    Rejected,
    TimedOut,
    Cancelled
};

USTRUCT(BlueprintType)
struct FAetherClientIdentitySnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString AccountID;
    UPROPERTY(BlueprintReadOnly) FString SessionID;
    UPROPERTY(BlueprintReadOnly) FString CharacterID;
    UPROPERTY(BlueprintReadOnly) FString CharacterName;
    UPROPERTY(BlueprintReadOnly) int32 CharacterLevel = 0;
    UPROPERTY(BlueprintReadOnly) FString MapID;
    UPROPERTY(BlueprintReadOnly) FString ZoneID;

    void Reset();
    bool HasAccount() const;
    bool HasCharacter() const;
};

USTRUCT(BlueprintType)
struct FAetherClientRequestState
{
    GENERATED_BODY()

    UPROPERTY() uint32 RequestID = 0;
    UPROPERTY(BlueprintReadOnly) EAetherClientRequestState State = EAetherClientRequestState::Pending;
    UPROPERTY(BlueprintReadOnly) double CreatedAtSeconds = 0.0;
    UPROPERTY(BlueprintReadOnly) double TimeoutSeconds = 15.0;
    UPROPERTY(BlueprintReadOnly) FString Operation;

    bool IsExpired(double NowSeconds) const;
};

USTRUCT(BlueprintType)
struct FAetherClientAssetBinding
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString AssetID;
    UPROPERTY(BlueprintReadOnly) FString RuntimePath;
    UPROPERTY(BlueprintReadOnly) bool bResolved = false;

    void Reset();
};

USTRUCT(BlueprintType)
struct FAetherClientRuntimeSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) EAetherClientConnectionState ConnectionState = EAetherClientConnectionState::Offline;
    UPROPERTY(BlueprintReadOnly) EAetherClientScreen Screen = EAetherClientScreen::None;
    UPROPERTY(BlueprintReadOnly) FAetherClientIdentitySnapshot Identity;
    UPROPERTY(BlueprintReadOnly) int64 ServerRevision = 0;
    UPROPERTY(BlueprintReadOnly) double LastServerTimeSeconds = 0.0;

    void Reset();
};
