#include "Items/AetherItemSubsystem.h"

bool UAetherItemSubsystem::RegisterDefinition(const FAetherItemDefinition& Definition)
{
    return ItemService.RegisterDefinition(Definition);
}

bool UAetherItemSubsystem::FindDefinition(const FAetherItemDefinitionId& DefinitionId, FAetherItemDefinition& OutDefinition) const
{
    return ItemService.FindDefinition(DefinitionId, OutDefinition);
}

bool UAetherItemSubsystem::GetInventory(const FAetherCharacterId& CharacterId, TArray<FAetherInventorySlot>& OutInventory) const
{
    return ItemService.GetInventory(CharacterId, OutInventory);
}

bool UAetherItemSubsystem::AddItem(const FAetherCharacterId& CharacterId, const FAetherItemDefinitionId& DefinitionId, int32 Quantity, TArray<FAetherInventorySlot>& OutInventory)
{
    return ItemService.AddItem(CharacterId, DefinitionId, Quantity, OutInventory);
}

bool UAetherItemSubsystem::RemoveItem(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 Quantity, TArray<FAetherInventorySlot>& OutInventory)
{
    return ItemService.RemoveItem(CharacterId, InstanceId, Quantity, OutInventory);
}

bool UAetherItemSubsystem::MoveItem(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 TargetSlot, TArray<FAetherInventorySlot>& OutInventory)
{
    return ItemService.MoveItem(CharacterId, InstanceId, TargetSlot, OutInventory);
}

bool UAetherItemSubsystem::SplitStack(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 Quantity, int32 TargetSlot, TArray<FAetherInventorySlot>& OutInventory)
{
    return ItemService.SplitStack(CharacterId, InstanceId, Quantity, TargetSlot, OutInventory);
}

bool UAetherItemSubsystem::MergeStacks(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& SourceInstanceId, const FAetherItemInstanceId& TargetInstanceId, TArray<FAetherInventorySlot>& OutInventory)
{
    return ItemService.MergeStacks(CharacterId, SourceInstanceId, TargetInstanceId, OutInventory);
}
