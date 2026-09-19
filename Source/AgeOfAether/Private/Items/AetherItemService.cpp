#include "Items/AetherItemService.h"

namespace
{
    bool SameStack(const FAetherItemInstance& A, const FAetherItemInstance& B)
    {
        return A.DefinitionId == B.DefinitionId
            && A.Binding == B.Binding
            && A.ItemLevel == B.ItemLevel
            && A.Durability == B.Durability;
    }
}

bool FAetherItemService::RegisterDefinition(const FAetherItemDefinition& Definition)
{
    if (!Definition.DefinitionId.IsValid() || Definition.InternalName.IsEmpty()
        || Definition.MaxStack <= 0 || Definition.Width <= 0 || Definition.Height <= 0
        || Definitions.Contains(Definition.DefinitionId))
    {
        return false;
    }

    Definitions.Add(Definition.DefinitionId, Definition);
    return true;
}

bool FAetherItemService::FindDefinition(const FAetherItemDefinitionId& DefinitionId, FAetherItemDefinition& OutDefinition) const
{
    const FAetherItemDefinition* Found = Definitions.Find(DefinitionId);
    if (!Found) return false;
    OutDefinition = *Found;
    return true;
}

bool FAetherItemService::GetInventory(const FAetherCharacterId& CharacterId, TArray<FAetherInventorySlot>& OutSlots) const
{
    const TArray<FAetherInventorySlot>* Found = Inventories.Find(CharacterId);
    if (!Found)
    {
        OutSlots.SetNum(MaxInventorySlots);
        for (int32 Index = 0; Index < OutSlots.Num(); ++Index) OutSlots[Index].SlotIndex = Index;
        return true;
    }

    OutSlots = *Found;
    return true;
}

bool FAetherItemService::CreateItemInstance(
    const FAetherCharacterId& CharacterId,
    const FAetherItemDefinitionId& DefinitionId,
    int32 Quantity,
    FAetherItemInstance& OutItem) const
{
    const FAetherItemDefinition* Definition = Definitions.Find(DefinitionId);
    if (!CharacterId.IsValid() || !Definition || Quantity <= 0 || Quantity > Definition->MaxStack) return false;

    OutItem = FAetherItemInstance();
    OutItem.InstanceId = FAetherItemInstanceId::NewId();
    OutItem.DefinitionId = DefinitionId;
    OutItem.OwnerCharacterId = CharacterId;
    OutItem.Quantity = Quantity;
    OutItem.ItemLevel = Definition->BaseLevel;
    OutItem.Durability = Definition->BaseDurability;
    OutItem.Binding = Definition->DefaultBinding;
    return true;
}

bool FAetherItemService::RestoreInventory(const FAetherCharacterId& CharacterId, const TArray<FAetherInventorySlot>& Slots)
{
    if (!CharacterId.IsValid() || Slots.Num() > MaxInventorySlots)
    {
        return false;
    }

    TArray<FAetherInventorySlot> Restored;
    Restored.SetNum(MaxInventorySlots);
    TSet<FAetherItemInstanceId> InstanceIds;

    for (int32 Index = 0; Index < Restored.Num(); ++Index)
    {
        Restored[Index].SlotIndex = Index;
    }

    for (const FAetherInventorySlot& Slot : Slots)
    {
        if (Slot.SlotIndex < 0 || Slot.SlotIndex >= MaxInventorySlots || !Slot.IsOccupied())
        {
            return false;
        }

        const FAetherItemDefinition* Definition = Definitions.Find(Slot.Item.DefinitionId);
        if (!Definition
            || Slot.Item.OwnerCharacterId != CharacterId
            || Slot.Item.Quantity <= 0
            || Slot.Item.Quantity > Definition->MaxStack
            || InstanceIds.Contains(Slot.Item.InstanceId))
        {
            return false;
        }

        Restored[Slot.SlotIndex] = Slot;
        InstanceIds.Add(Slot.Item.InstanceId);
    }

    Inventories.Add(CharacterId, MoveTemp(Restored));
    return true;
}

bool FAetherItemService::AddItem(
    const FAetherCharacterId& CharacterId,
    const FAetherItemDefinitionId& DefinitionId,
    int32 Quantity,
    TArray<FAetherInventorySlot>& OutInventory)
{
    const FAetherItemDefinition* Definition = Definitions.Find(DefinitionId);
    if (!CharacterId.IsValid() || !Definition || Quantity <= 0) return false;

    TArray<FAetherInventorySlot> Working;
    GetInventory(CharacterId, Working);

    int32 Remaining = Quantity;

    for (FAetherInventorySlot& Slot : Working)
    {
        if (!Slot.IsOccupied() || Slot.Item.DefinitionId != DefinitionId || Slot.Item.Quantity >= Definition->MaxStack) continue;

        const int32 Added = FMath::Min(Remaining, Definition->MaxStack - Slot.Item.Quantity);
        Slot.Item.Quantity += Added;
        Remaining -= Added;
        if (Remaining == 0)
        {
            Inventories.Add(CharacterId, Working);
            OutInventory = Working;
            return true;
        }
    }

    while (Remaining > 0)
    {
        int32 EmptySlot = INDEX_NONE;
        for (int32 Index = 0; Index < Working.Num(); ++Index)
        {
            if (!Working[Index].IsOccupied())
            {
                EmptySlot = Index;
                break;
            }
        }

        if (EmptySlot == INDEX_NONE)
        {
            OutInventory = Working;
            return false;
        }

        const int32 StackQuantity = FMath::Min(Remaining, Definition->MaxStack);
        FAetherItemInstance NewItem;
        if (!CreateItemInstance(CharacterId, DefinitionId, StackQuantity, NewItem))
        {
            OutInventory = Working;
            return false;
        }

        Working[EmptySlot].SlotIndex = EmptySlot;
        Working[EmptySlot].Item = NewItem;
        Remaining -= StackQuantity;
    }

    Inventories.Add(CharacterId, Working);
    OutInventory = Working;
    return true;
}

bool FAetherItemService::RemoveItem(
    const FAetherCharacterId& CharacterId,
    const FAetherItemInstanceId& InstanceId,
    int32 Quantity,
    TArray<FAetherInventorySlot>& OutInventory)
{
    int32 SlotIndex = INDEX_NONE;
    if (Quantity <= 0 || !FindSlot(CharacterId, InstanceId, SlotIndex)) return false;

    TArray<FAetherInventorySlot>& Inventory = Inventories[CharacterId];
    FAetherInventorySlot& Slot = Inventory[SlotIndex];
    if (Quantity > Slot.Item.Quantity) return false;

    Slot.Item.Quantity -= Quantity;
    if (Slot.Item.Quantity == 0)
    {
        Slot = FAetherInventorySlot();
        Slot.SlotIndex = SlotIndex;
    }

    OutInventory = Inventory;
    return true;
}

bool FAetherItemService::MoveItem(
    const FAetherCharacterId& CharacterId,
    const FAetherItemInstanceId& InstanceId,
    int32 TargetSlot,
    TArray<FAetherInventorySlot>& OutInventory)
{
    int32 SourceSlot = INDEX_NONE;
    if (!FindSlot(CharacterId, InstanceId, SourceSlot) || !IsSlotValid(TargetSlot)) return false;

    TArray<FAetherInventorySlot>& Inventory = Inventories[CharacterId];
    if (SourceSlot == TargetSlot)
    {
        OutInventory = Inventory;
        return true;
    }

    if (Inventory[TargetSlot].IsOccupied()) return false;

    Swap(Inventory[SourceSlot], Inventory[TargetSlot]);
    Inventory[SourceSlot].SlotIndex = SourceSlot;
    Inventory[TargetSlot].SlotIndex = TargetSlot;
    OutInventory = Inventory;
    return true;
}

bool FAetherItemService::SplitStack(
    const FAetherCharacterId& CharacterId,
    const FAetherItemInstanceId& InstanceId,
    int32 Quantity,
    int32 TargetSlot,
    TArray<FAetherInventorySlot>& OutInventory)
{
    int32 SourceSlot = INDEX_NONE;
    if (!FindSlot(CharacterId, InstanceId, SourceSlot) || !IsSlotValid(TargetSlot) || Quantity <= 0) return false;

    TArray<FAetherInventorySlot>& Inventory = Inventories[CharacterId];
    FAetherInventorySlot& Source = Inventory[SourceSlot];
    if (Inventory[TargetSlot].IsOccupied() || Quantity >= Source.Item.Quantity) return false;

    FAetherItemInstance NewItem = Source.Item;
    NewItem.InstanceId = FAetherItemInstanceId::NewId();
    NewItem.Quantity = Quantity;
    Source.Item.Quantity -= Quantity;

    Inventory[TargetSlot].SlotIndex = TargetSlot;
    Inventory[TargetSlot].Item = NewItem;
    OutInventory = Inventory;
    return true;
}

bool FAetherItemService::MergeStacks(
    const FAetherCharacterId& CharacterId,
    const FAetherItemInstanceId& SourceInstanceId,
    const FAetherItemInstanceId& TargetInstanceId,
    TArray<FAetherInventorySlot>& OutInventory)
{
    int32 SourceSlot = INDEX_NONE;
    int32 TargetSlot = INDEX_NONE;
    if (!FindSlot(CharacterId, SourceInstanceId, SourceSlot)
        || !FindSlot(CharacterId, TargetInstanceId, TargetSlot)
        || SourceSlot == TargetSlot) return false;

    TArray<FAetherInventorySlot>& Inventory = Inventories[CharacterId];
    FAetherItemInstance& Source = Inventory[SourceSlot].Item;
    FAetherItemInstance& Target = Inventory[TargetSlot].Item;

    FAetherItemDefinition Definition;
    if (!FindDefinition(Target.DefinitionId, Definition)
        || !SameStack(Source, Target)
        || Target.Quantity >= Definition.MaxStack) return false;

    const int32 Moved = FMath::Min(Definition.MaxStack - Target.Quantity, Source.Quantity);
    Target.Quantity += Moved;
    Source.Quantity -= Moved;

    if (Source.Quantity == 0)
    {
        Inventory[SourceSlot] = FAetherInventorySlot();
        Inventory[SourceSlot].SlotIndex = SourceSlot;
    }

    OutInventory = Inventory;
    return true;
}

int32 FAetherItemService::NumDefinitions() const
{
    return Definitions.Num();
}

int32 FAetherItemService::NumOccupiedSlots(const FAetherCharacterId& CharacterId) const
{
    const TArray<FAetherInventorySlot>* Inventory = Inventories.Find(CharacterId);
    if (!Inventory) return 0;

    int32 Count = 0;
    for (const FAetherInventorySlot& Slot : *Inventory) Count += Slot.IsOccupied() ? 1 : 0;
    return Count;
}

bool FAetherItemService::FindSlot(
    const FAetherCharacterId& CharacterId,
    const FAetherItemInstanceId& InstanceId,
    int32& OutSlot) const
{
    const TArray<FAetherInventorySlot>* Inventory = Inventories.Find(CharacterId);
    if (!Inventory) return false;

    for (int32 Index = 0; Index < Inventory->Num(); ++Index)
    {
        if ((*Inventory)[Index].IsOccupied() && (*Inventory)[Index].Item.InstanceId == InstanceId)
        {
            OutSlot = Index;
            return true;
        }
    }

    return false;
}

bool FAetherItemService::IsSlotValid(int32 SlotIndex)
{
    return SlotIndex >= 0 && SlotIndex < MaxInventorySlots;
}
