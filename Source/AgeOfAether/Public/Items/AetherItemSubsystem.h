#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/AetherItemService.h"
#include "Items/AetherItemDefinitionDataAsset.h"
#include "AetherItemSubsystem.generated.h"
UCLASS()
class AGEOFAETHER_API UAetherItemSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool RegisterDefinition(const FAetherItemDefinition& Definition);
    bool RegisterDefinitionAsset(const UAetherItemDefinitionDataAsset* DefinitionAsset);
    bool FindDefinition(const FAetherItemDefinitionId& DefinitionId, FAetherItemDefinition& OutDefinition) const;
    bool GetInventory(const FAetherCharacterId& CharacterId, TArray<FAetherInventorySlot>& OutInventory) const;

    bool AddItem(const FAetherCharacterId& CharacterId, const FAetherItemDefinitionId& DefinitionId, int32 Quantity, TArray<FAetherInventorySlot>& OutInventory);
    bool RemoveItem(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 Quantity, TArray<FAetherInventorySlot>& OutInventory);
    bool MoveItem(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 TargetSlot, TArray<FAetherInventorySlot>& OutInventory);
    bool SplitStack(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& InstanceId, int32 Quantity, int32 TargetSlot, TArray<FAetherInventorySlot>& OutInventory);
    bool MergeStacks(const FAetherCharacterId& CharacterId, const FAetherItemInstanceId& SourceInstanceId, const FAetherItemInstanceId& TargetInstanceId, TArray<FAetherInventorySlot>& OutInventory);
    FAetherItemService& GetItemService() { return ItemService; }
    const FAetherItemService& GetItemService() const { return ItemService; }

private:
    FAetherItemService ItemService;
};
