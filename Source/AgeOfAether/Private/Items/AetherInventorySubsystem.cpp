#include "Items/AetherInventorySubsystem.h"

FString UAetherInventorySubsystem::CharacterKey(const FAetherCharacterId& Id){ return Id.Value.ToLower(); }

const FAetherItemDefinition* UAetherInventorySubsystem::ResolveDefinition(const FString& DefinitionID) const
{
    if (!ItemRegistry) return nullptr;
    FAetherItemDefinition Def;
    if (!ItemRegistry->Resolve(DefinitionID, Def)) return nullptr;
    static thread_local FAetherItemDefinition CachedDefinition;
    CachedDefinition = MoveTemp(Def);
    return &CachedDefinition;
}

FAetherInventoryItem* UAetherInventorySubsystem::FindItem(FAetherInventoryState& State,const FString& InstanceID)
{
    for(FAetherInventoryItem& Item:State.Items) if(Item.Snapshot.InstanceID.Equals(InstanceID,ESearchCase::IgnoreCase)) return &Item;
    return nullptr;
}

bool UAetherInventorySubsystem::AddItem(const FAetherCharacterId& CharacterId,const FString& DefinitionID,int32 Quantity,FAetherInventoryItem& OutItem,EAetherInventoryResult& OutResult)
{
    OutItem=FAetherInventoryItem{}; OutResult=EAetherInventoryResult::InvalidRequest;
    if(!CharacterId.IsValid()||Quantity<=0){OutResult=EAetherInventoryResult::InvalidQuantity;return false;}
    const FAetherItemDefinition* Def=ResolveDefinition(DefinitionID);
    if(!Def){OutResult=EAetherInventoryResult::DefinitionNotFound;return false;}
    FAetherInventoryState& State=States.FindOrAdd(CharacterKey(CharacterId)); State.CharacterId=CharacterId; State.Capacity=64;
    int32 Remaining=Quantity;
    if(Def->MaxStack<=0){OutResult=EAetherInventoryResult::CannotStack;return false;}
    for(FAetherInventoryItem& Item:State.Items){
        if(Item.Snapshot.DefinitionID.Equals(Def->DefinitionID,ESearchCase::IgnoreCase)&&Item.Snapshot.Quantity<Def->MaxStack){
            const int32 Add=FMath::Min(Remaining,Def->MaxStack-Item.Snapshot.Quantity); Item.Snapshot.Quantity+=Add; Remaining-=Add; OutItem=Item; if(Remaining==0){OutResult=EAetherInventoryResult::Accepted;return true;}
        }
    }
    while(Remaining>0 && State.Items.Num()<State.Capacity){
        FAetherInventoryItem Item; Item.SlotIndex=State.Items.Num(); Item.Snapshot.InstanceID=FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower); Item.Snapshot.DefinitionID=Def->DefinitionID; Item.Snapshot.Quantity=FMath::Min(Remaining,Def->MaxStack); Item.Snapshot.ItemLevel=Def->ItemLevel; Item.Snapshot.Durability=Def->MaxDurability; State.Items.Add(Item); Remaining-=Item.Snapshot.Quantity; OutItem=Item;
    }
    OutResult=Remaining==0?EAetherInventoryResult::Accepted:EAetherInventoryResult::InventoryFull; return Remaining==0;
}

bool UAetherInventorySubsystem::RemoveItem(const FAetherCharacterId& CharacterId,const FString& InstanceID,int32 Quantity,EAetherInventoryResult& OutResult)
{
    OutResult=EAetherInventoryResult::InvalidRequest; if(Quantity<=0){OutResult=EAetherInventoryResult::InvalidQuantity;return false;}
    FAetherInventoryState* State=States.Find(CharacterKey(CharacterId)); if(!State){OutResult=EAetherInventoryResult::ItemNotFound;return false;}
    FAetherInventoryItem* Item=FindItem(*State,InstanceID); if(!Item){OutResult=EAetherInventoryResult::ItemNotFound;return false;}
    if(Item->Snapshot.Quantity<Quantity){OutResult=EAetherInventoryResult::InsufficientQuantity;return false;}
    Item->Snapshot.Quantity-=Quantity; if(Item->Snapshot.Quantity==0) State->Items.RemoveAll([&](const FAetherInventoryItem& X){return X.Snapshot.InstanceID==InstanceID;});
    OutResult=EAetherInventoryResult::Accepted; return true;
}

bool UAetherInventorySubsystem::SplitStack(const FAetherCharacterId& CharacterId,const FString& InstanceID,int32 Quantity,FAetherInventoryItem& OutItem,EAetherInventoryResult& OutResult)
{
    OutItem={}; OutResult=EAetherInventoryResult::InvalidRequest;
    FAetherInventoryState* State=States.Find(CharacterKey(CharacterId)); if(!State){OutResult=EAetherInventoryResult::ItemNotFound;return false;}
    FAetherInventoryItem* Item=FindItem(*State,InstanceID); if(!Item||Quantity<=0||Quantity>=Item->Snapshot.Quantity){OutResult=EAetherInventoryResult::InvalidQuantity;return false;}
    if(State->Items.Num()>=State->Capacity){OutResult=EAetherInventoryResult::InventoryFull;return false;}
    Item->Snapshot.Quantity-=Quantity; OutItem=*Item; OutItem.Snapshot.InstanceID=FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower); OutItem.Snapshot.Quantity=Quantity; OutItem.SlotIndex=State->Items.Num(); State->Items.Add(OutItem); OutResult=EAetherInventoryResult::Accepted; return true;
}

bool UAetherInventorySubsystem::EquipItem(const FAetherCharacterId& CharacterId,const FString& InstanceID,EAetherInventoryResult& OutResult)
{
    OutResult=EAetherInventoryResult::InvalidRequest; FAetherInventoryState* State=States.Find(CharacterKey(CharacterId)); if(!State){OutResult=EAetherInventoryResult::ItemNotFound;return false;}
    FAetherInventoryItem* Item=FindItem(*State,InstanceID); if(!Item){OutResult=EAetherInventoryResult::ItemNotFound;return false;}
    const FAetherItemDefinition* Def=ResolveDefinition(Item->Snapshot.DefinitionID); if(!Def){OutResult=EAetherInventoryResult::DefinitionNotFound;return false;}
    if(Def->EquipmentSlot==EAetherEquipmentSlot::None||Def->Category!=EAetherItemCategory::Equipment){OutResult=EAetherInventoryResult::CannotEquip;return false;}
    if(State->Equipment.Equipped.Contains(Def->EquipmentSlot)){OutResult=EAetherInventoryResult::AlreadyEquipped;return false;}
    State->Equipment.Equipped.Add(Def->EquipmentSlot,*Item); State->Items.RemoveAll([&](const FAetherInventoryItem& X){return X.Snapshot.InstanceID==InstanceID;}); OutResult=EAetherInventoryResult::Accepted; return true;
}

bool UAetherInventorySubsystem::UnequipSlot(const FAetherCharacterId& CharacterId,EAetherEquipmentSlot Slot,EAetherInventoryResult& OutResult)
{
    OutResult=EAetherInventoryResult::InvalidRequest; FAetherInventoryState* State=States.Find(CharacterKey(CharacterId)); if(!State){OutResult=EAetherInventoryResult::ItemNotFound;return false;}
    FAetherInventoryItem* Equipped=State->Equipment.Equipped.Find(Slot); if(!Equipped){OutResult=EAetherInventoryResult::InvalidSlot;return false;}
    if(State->Items.Num()>=State->Capacity){OutResult=EAetherInventoryResult::InventoryFull;return false;}
    Equipped->SlotIndex=State->Items.Num(); State->Items.Add(*Equipped); State->Equipment.Equipped.Remove(Slot); OutResult=EAetherInventoryResult::Accepted; return true;
}

FAetherInventoryState UAetherInventorySubsystem::GetState(const FAetherCharacterId& CharacterId) const { const FAetherInventoryState* S=States.Find(CharacterKey(CharacterId)); return S?*S:FAetherInventoryState{}; }
void UAetherInventorySubsystem::ResetCharacter(const FAetherCharacterId& CharacterId){States.Remove(CharacterKey(CharacterId));}
