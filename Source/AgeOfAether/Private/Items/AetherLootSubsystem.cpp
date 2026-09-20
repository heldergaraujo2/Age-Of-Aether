#include "Items/AetherLootSubsystem.h"
FAetherLootRollResult UAetherLootSubsystem::Roll(const UAetherLootTable* Table) const
{
 FAetherLootRollResult R; if(!Table)return R;
 for(const FAetherLootEntry& E:Table->Entries) if(FMath::FRand()<=E.DropChance){R.ItemIDs.Add(E.ItemID);R.Quantities.Add(FMath::RandRange(E.MinQuantity,E.MaxQuantity));}
 return R;
}
bool UAetherLootSubsystem::GrantToCharacter(const FAetherCharacterId& CharacterId,const UAetherLootTable* Table,UAetherInventorySubsystem* Inventory,FAetherLootRollResult& OutRoll)
{
 if(!Inventory||!Table)return false; OutRoll=Roll(Table); bool bAll=true;
 for(int32 i=0;i<OutRoll.ItemIDs.Num();++i){FAetherInventoryItem Item;EAetherInventoryResult Result; if(!Inventory->AddItem(CharacterId,OutRoll.ItemIDs[i],OutRoll.Quantities[i],Item,Result))bAll=false;}
 return bAll;
}
