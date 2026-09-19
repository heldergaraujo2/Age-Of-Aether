#pragma once

#include "CoreMinimal.h"
#include "Accounts/AetherAccountSessionTypes.h"
#include "Characters/AetherCharacterTypes.h"
#include "AetherSocialTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherFriendState : uint8
{
    None,
    PendingOutgoing,
    PendingIncoming,
    Accepted,
    Blocked
};

UENUM(BlueprintType)
enum class EAetherPartyRole : uint8
{
    Member,
    Leader
};

UENUM(BlueprintType)
enum class EAetherGuildRole : uint8
{
    Member,
    Officer,
    Leader
};

UENUM(BlueprintType)
enum class EAetherSocialChannel : uint8
{
    Local,
    Party,
    Guild,
    Whisper,
    System
};

UENUM(BlueprintType)
enum class EAetherSocialResult : uint8
{
    Accepted,
    InvalidRequest,
    NotAuthenticated,
    CharacterRequired,
    CharacterNotFound,
    NotOwned,
    AlreadyFriends,
    RequestPending,
    RequestNotFound,
    Blocked,
    CannotTargetSelf,
    NotPartyMember,
    NotPartyLeader,
    PartyNotFound,
    PartyFull,
    AlreadyInParty,
    InvitePending,
    NotInvited,
    AlreadyInGuild,
    GuildNotFound,
    GuildFull,
    NotGuildMember,
    NotGuildLeader,
    GuildNameUnavailable,
    ChatRateLimited,
    ChatNotAllowed,
    InvalidMessage,
    TargetNotFound
};

USTRUCT(BlueprintType)
struct FAetherSocialPartyId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherSocialPartyId NewId()
    {
        FAetherSocialPartyId Id;
        Id.Value = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        return Id;
    }

    bool operator==(const FAetherSocialPartyId& Other) const { return Value == Other.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FAetherSocialPartyId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherGuildId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherGuildId NewId()
    {
        FAetherGuildId Id;
        Id.Value = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        return Id;
    }

    bool operator==(const FAetherGuildId& Other) const { return Value == Other.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FAetherGuildId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherSocialRelation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    EAetherFriendState State = EAetherFriendState::None;
};

USTRUCT(BlueprintType)
struct FAetherPartyMember
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId CharacterId;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    EAetherPartyRole Role = EAetherPartyRole::Member;
};

USTRUCT(BlueprintType)
struct FAetherPartyState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherSocialPartyId PartyId;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherPartyMember> Members;
};

USTRUCT(BlueprintType)
struct FAetherGuildMember
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId CharacterId;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    EAetherGuildRole Role = EAetherGuildRole::Member;
};

USTRUCT(BlueprintType)
struct FAetherGuildState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherGuildId GuildId;

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherGuildMember> Members;
};

USTRUCT(BlueprintType)
struct FAetherSocialOperation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherSocialResult Result = EAetherSocialResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    FAetherPartyState Party;

    UPROPERTY(BlueprintReadOnly)
    FAetherGuildState Guild;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherSocialRelation> Friends;
};

USTRUCT(BlueprintType)
struct FAetherChatMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherSocialChannel Channel = EAetherSocialChannel::System;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId SenderAccountId;

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId SenderCharacterId;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId TargetAccountId;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    double ServerTimeSeconds = 0.0;
};

USTRUCT(BlueprintType)
struct FAetherSocialConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxPartyMembers = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxGuildMembers = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxMessageLength = 200;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ChatCooldownSeconds = 0.75f;
};
