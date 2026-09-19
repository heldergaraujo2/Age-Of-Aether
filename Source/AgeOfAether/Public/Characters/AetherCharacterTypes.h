#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherCharacterClass : uint8
{
    Warrior,
    Mage,
    Archer,
    Cleric
};

UENUM(BlueprintType)
enum class EAetherCharacterOperationResult : uint8
{
    Accepted,
    InvalidRequest,
    NotAuthenticated,
    CharacterNotFound,
    NotOwned,
    InvalidName,
    NameUnavailable,
    CharacterLimitReached,
    CharacterDisabled,
    CharacterDeleted,
    AlreadySelected,
    AnotherCharacterSelected
};

UENUM(BlueprintType)
enum class EAetherCharacterCombatState : uint8
{
    Alive,
    Dead
};

UENUM(BlueprintType)
enum class EAetherCharacterStatus : uint8
{
    Created,
    Available,
    Selected,
    Active,
    Offline,
    Disabled,
    Deleted
};

USTRUCT(BlueprintType)
struct FAetherCharacterId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherCharacterId NewId()
    {
        FAetherCharacterId Id;
        Id.Value = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        return Id;
    }

    bool operator==(const FAetherCharacterId& Other) const { return Value == Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FAetherCharacterId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherBaseStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 Strength = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Agility = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Vitality = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Energy = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Command = 0;
};

USTRUCT(BlueprintType)
struct FAetherDerivedStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float MaxHealth = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxMana = 50.0f;

    UPROPERTY(BlueprintReadOnly)
    float AttackMin = 10.0f;

    UPROPERTY(BlueprintReadOnly)
    float AttackMax = 15.0f;

    UPROPERTY(BlueprintReadOnly)
    float Defense = 5.0f;

    UPROPERTY(BlueprintReadOnly)
    float Resistance = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float MoveSpeed = 600.0f;
};

USTRUCT(BlueprintType)
struct FAetherCharacterRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId CharacterId;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    EAetherCharacterClass CharacterClass = EAetherCharacterClass::Warrior;

    UPROPERTY(BlueprintReadOnly)
    EAetherCharacterStatus Status = EAetherCharacterStatus::Created;

    UPROPERTY(BlueprintReadOnly)
    int32 Level = 1;

    UPROPERTY(BlueprintReadOnly)
    int64 Experience = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 UnspentStatPoints = 0;

    UPROPERTY(BlueprintReadOnly)
    FAetherBaseStats BaseStats;

    UPROPERTY(BlueprintReadOnly)
    FAetherDerivedStats DerivedStats;

    UPROPERTY(BlueprintReadOnly)
    float CurrentHealth = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    float CurrentShield = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    EAetherCharacterCombatState CombatState = EAetherCharacterCombatState::Alive;

    UPROPERTY(BlueprintReadOnly)
    FVector WorldLocation = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    FRotator WorldRotation = FRotator::ZeroRotator;
};
