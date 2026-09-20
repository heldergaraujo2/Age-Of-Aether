#include "Items/AetherLootTypes.h"
bool UAetherLootTable::Validate(FString& OutError) const
{
    OutError.Reset();
    if(LootTableID.TrimStartAndEnd().IsEmpty()){OutError=TEXT("LootTableID is required");return false;}
    for(const FAetherLootEntry& E:Entries) if(!E.IsValid()){OutError=FString::Printf(TEXT("Invalid loot entry: %s"),*E.ItemID);return false;}
    return true;
}
