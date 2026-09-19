#pragma once

#include "CoreMinimal.h"
#include "Items/AetherItemTypes.h"

class FAetherItemService
{
public:
    static constexpr int32 MaxInventorySlots = AETHER_DEFAULT_INVENTORY_SLOTS;

    bool RegisterDefinition(const FAetherItemDefinition& Definition);
    bool FindDefinition(const FAetherItemDefinitionId& DefinitionId, FAetherItemDefinition& OutDefinition) const;

    bool GetInventory(const FAetherCharacterId& CharacterId, TArray<FAetherInventorySlot>& OutSlots) const;
    bool RestoreInventory(const FAetherCharacterId& CharacterId, const TArray<FAetherInventorySlot>& Slots);

    bool AddItem(const FAetherCharacterId& CharacterId, const FAetherItemDefinitionId& DefinitionId, int32 Quantity, TArray<FAetherInventorySlot>& OutInventory);
    bool RemoveItem(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 Quantity, TArray<FAetherInventorySlot>& OutInventory);
    bool MoveItem(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 TargetSlot, TArray<FAetherInventorySlot>& OutInventory);
    bool SplitStack(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 Quantity, int32 TargetSlot, TArray<FAetherInventorySlot>& OutInventory);
    bool MergeStacks(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& SourceInstanceId, const FAetherItemInstanceId& TargetInstanceId, TArray<FAetherInventorySlot>& OutInventory);

    int32 NumDefinitions() const;
    int32 NumOccupiedSlots(const FAetherCharacterId& CharacterId) const;

private:
    bool CreateItemInstance(const FAetherCharacterId& CharacterId, const FAetherItemDefinitionId& DefinitionId, int32 Quantity, FAetherItemInstance& OutItem) const;
    bool FindSlot(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32& OutSlot) const;
    static bool IsSlotValid(int32 SlotIndex);

    TMap<FAetherItemDefinitionId, FAetherItemDefinition> Definitions;
    TMap<FAetherCharacterId, TArray<FAetherInventorySlot>> Inventories;
};
