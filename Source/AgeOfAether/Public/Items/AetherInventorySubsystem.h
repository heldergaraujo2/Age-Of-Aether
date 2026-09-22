#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/AetherInventoryTypes.h"
#include "Data/AetherItemRegistry.h"
#include "AetherInventorySubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherInventorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    bool AddItem(const FAetherCharacterId& CharacterId, const FString& DefinitionID, int32 Quantity, FAetherInventoryItem& OutItem, EAetherInventoryResult& OutResult);
    bool RemoveItem(const FAetherCharacterId& CharacterId, const FString& InstanceID, int32 Quantity, EAetherInventoryResult& OutResult);
    bool SplitStack(const FAetherCharacterId& CharacterId, const FString& InstanceID, int32 Quantity, FAetherInventoryItem& OutItem, EAetherInventoryResult& OutResult);
    bool EquipItem(const FAetherCharacterId& CharacterId, const FString& InstanceID, EAetherInventoryResult& OutResult);
    bool UnequipSlot(const FAetherCharacterId& CharacterId, EAetherEquipmentSlot Slot, EAetherInventoryResult& OutResult);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Inventory")
    FAetherInventoryState GetState(const FAetherCharacterId& CharacterId) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Inventory")
    void ResetCharacter(const FAetherCharacterId& CharacterId);

    void SetItemRegistry(const FAetherItemRegistry* InRegistry) { ItemRegistry = InRegistry; }
private:
    TMap<FString, FAetherInventoryState> States;
    const FAetherItemRegistry* ItemRegistry = nullptr;
    static FString CharacterKey(const FAetherCharacterId& Id);
    FAetherInventoryItem* FindItem(FAetherInventoryState& State, const FString& InstanceID);
    const FAetherDataItemDefinition* ResolveDefinition(const FString& DefinitionID) const;
};