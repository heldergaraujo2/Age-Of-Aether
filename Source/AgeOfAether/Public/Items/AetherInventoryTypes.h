#pragma once
#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Data/AetherItemDataTypes.h"
#include "AetherItemTypes.h"
#include "AetherInventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FAetherInventoryItem
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FAetherItemInstanceSnapshot Snapshot;
    UPROPERTY(BlueprintReadOnly) int32 SlotIndex = INDEX_NONE;
    bool IsValid() const { return !Snapshot.InstanceID.IsEmpty() && !Snapshot.DefinitionID.IsEmpty() && Snapshot.Quantity > 0; }
};

USTRUCT(BlueprintType)
struct FAetherEquipmentState
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) TMap<EAetherEquipmentSlot, FAetherInventoryItem> Equipped;
};

USTRUCT(BlueprintType)
struct FAetherInventoryState
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FAetherCharacterId CharacterId;
    UPROPERTY(BlueprintReadOnly) int32 Capacity = 64;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherInventoryItem> Items;
    UPROPERTY(BlueprintReadOnly) FAetherEquipmentState Equipment;
};

UENUM(BlueprintType)
enum class EAetherInventoryResult : uint8
{
    Accepted, InvalidRequest, ItemNotFound, DefinitionNotFound, InventoryFull,
    InvalidQuantity, InsufficientQuantity, CannotStack, InvalidSlot, RequirementFailed,
    CannotEquip, AlreadyEquipped, NotAuthenticated, NotOwned
};