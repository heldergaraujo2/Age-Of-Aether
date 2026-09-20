#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/AetherLootTypes.h"
#include "Items/AetherInventorySubsystem.h"
#include "AetherLootSubsystem.generated.h"

USTRUCT(BlueprintType) struct FAetherLootRollResult { GENERATED_BODY() UPROPERTY(BlueprintReadOnly) TArray<FString> ItemIDs; UPROPERTY(BlueprintReadOnly) TArray<int32> Quantities; };

UCLASS() class AGEOFAETHER_API UAetherLootSubsystem : public UGameInstanceSubsystem
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Loot") FAetherLootRollResult Roll(const UAetherLootTable* Table) const;
 bool GrantToCharacter(const FAetherCharacterId& CharacterId,const UAetherLootTable* Table,UAetherInventorySubsystem* Inventory,FAetherLootRollResult& OutRoll);
};
