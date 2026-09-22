#pragma once

#include "CoreMinimal.h"
#include "AetherWorldTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherWorldZoneType : uint8
{
    Safe,
    PvP,
    Event,
    Dungeon,
    City
};

UENUM(BlueprintType)
enum class EAetherWorldTransitionResult : uint8
{
    Accepted,
    InvalidRequest,
    NotAuthenticated,
    CharacterNotSelected,
    ZoneNotFound,
    ZoneDisabled,
    LevelRequirement,
    PortalNotFound,
    PortalDisabled,
    InvalidSourceZone,
    InvalidSpawn
};

USTRUCT(BlueprintType)
struct FAetherWorldZoneId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    bool operator==(const FAetherWorldZoneId& Other) const { return Value == Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FAetherWorldZoneId& Id)
{
    return GetTypeHash(Id.Value);
}

USTRUCT(BlueprintType)
struct FAetherWorldSpawnPoint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SpawnId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherWorldZoneId ZoneId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinimumLevel = 1;
};

USTRUCT(BlueprintType)
struct FAetherWorldZone
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherWorldZoneId ZoneId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAetherWorldZoneType Type = EAetherWorldZoneType::Safe;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinimumLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bAllowCombat = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RecommendedLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FAetherWorldPortal
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString PortalId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherWorldZoneId SourceZoneId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherWorldZoneId TargetZoneId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinimumLevel = 1;
};

USTRUCT(BlueprintType)
struct FAetherWorldTransitionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherWorldTransitionResult Result = EAetherWorldTransitionResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    FAetherWorldZoneId SourceZoneId;

    UPROPERTY(BlueprintReadOnly)
    FAetherWorldZoneId TargetZoneId;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    FRotator TargetRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadOnly)
    FString PortalId;

    bool IsAccepted() const
    {
        return Result == EAetherWorldTransitionResult::Accepted;
    }
};

USTRUCT(BlueprintType)
struct FAetherWorldConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxWorldZones = 1024;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxSpawnsPerZone = 256;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxPortals = 4096;
};
