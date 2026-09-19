#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Items/AetherItemTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherItemCategory : uint8
{
    Weapon,
    Armor,
    Accessory,
    Consumable,
    Material,
    Quest,
    Miscellaneous
};

UENUM(BlueprintType)
enum class EAetherEquipmentSlot : uint8
{
    None,
    Head,
    Chest,
    Legs,
    Hands,
    Feet,
    MainHand,
    OffHand,
    Accessory
};

UENUM(BlueprintType)
enum class EAetherItemBinding : uint8
{
    None,
    BindOnPickup,
    BindOnEquip,
    CharacterBound
};

UENUM(BlueprintType)
enum class EAetherInventoryOperationResult : uint8
{
    Accepted,
    InvalidRequest,
    NotAuthenticated,
    CharacterNotFound,
    NotOwned,
    ItemNotFound,
    DefinitionNotFound,
    InvalidSlot,
    InvalidQuantity,
    InsufficientQuantity,
    InventoryFull,
    CannotStack,
    CannotMerge,
    CannotSplit,
    SourceAndTargetMismatch
};

USTRUCT(BlueprintType)
struct FAetherItemDefinitionId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherItemDefinitionId FromString(const FString& InValue)
    {
        FAetherItemDefinitionId Id;
        Id.Value = InValue.TrimStartAndEnd();
        return Id;
    }

    bool operator==(const FAetherItemDefinitionId& Other) const { return Value == Other.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FAetherItemDefinitionId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherItemInstanceId
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }

    static FAetherItemInstanceId NewId()
    {
        FAetherItemInstanceId Id;
        Id.Value = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        return Id;
    }

    bool operator==(const FAetherItemInstanceId& Other) const { return Value == Other.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FAetherItemInstanceId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherItemDefinition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherItemDefinitionId DefinitionId;

    UPROPERTY(BlueprintReadOnly)
    FString InternalName;

    UPROPERTY(BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    EAetherItemCategory Category = EAetherItemCategory::Miscellaneous;

    UPROPERTY(BlueprintReadOnly)
    EAetherEquipmentSlot EquipmentSlot = EAetherEquipmentSlot::None;

    UPROPERTY(BlueprintReadOnly)
    int32 Width = 1;

    UPROPERTY(BlueprintReadOnly)
    int32 Height = 1;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxStack = 1;

    UPROPERTY(BlueprintReadOnly)
    int32 BaseLevel = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 BaseDurability = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherItemBinding DefaultBinding = EAetherItemBinding::None;

    UPROPERTY(BlueprintReadOnly)
    int32 AllowedClassMask = 0x0F;
};

USTRUCT(BlueprintType)
struct FAetherItemInstance
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherItemInstanceId InstanceId;

    UPROPERTY(BlueprintReadOnly)
    FAetherItemDefinitionId DefinitionId;

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId OwnerCharacterId;

    UPROPERTY(BlueprintReadOnly)
    int32 Quantity = 1;

    UPROPERTY(BlueprintReadOnly)
    int32 ItemLevel = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 Durability = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherItemBinding Binding = EAetherItemBinding::None;
};

USTRUCT(BlueprintType)
struct FAetherInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 SlotIndex = INDEX_NONE;

    UPROPERTY(BlueprintReadOnly)
    FAetherItemInstance Item;

    bool IsOccupied() const { return Item.InstanceId.IsValid(); }
};

static constexpr int32 AETHER_DEFAULT_INVENTORY_SLOTS = 64;
