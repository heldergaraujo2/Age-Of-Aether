#include "UI/AetherUITypes.h"

bool FAetherUIItemEntry::IsValid() const
{
    return !InstanceID.TrimStartAndEnd().IsEmpty()
        && !DefinitionID.TrimStartAndEnd().IsEmpty()
        && Quantity > 0
        && SlotIndex >= 0
        && ItemLevel >= 0
        && EnhancementLevel >= 0
        && FMath::IsFinite(Durability)
        && Durability >= 0.0f;
}

bool FAetherUIEquipmentEntry::IsValid() const
{
    return !SlotID.TrimStartAndEnd().IsEmpty() && (!bOccupied || Item.IsValid());
}

bool FAetherUINotification::IsExpired(double NowSeconds) const
{
    return DurationSeconds >= 0.0 && NowSeconds >= CreatedAtSeconds + DurationSeconds;
}

bool FAetherUISettings::IsValid() const
{
    return FMath::IsFinite(UIScale) && UIScale >= 0.5f && UIScale <= 2.0f
        && !Language.TrimStartAndEnd().IsEmpty();
}

void FAetherUIRuntimeSnapshot::Reset()
{
    CurrentScreen = EAetherUIScreen::None;
    PreviousScreen = EAetherUIScreen::None;
    WindowMode = EAetherUIWindowMode::Fullscreen;
    bModalOpen = false;
    ModalID.Reset();
    InventoryCapacity = 0;
    Inventory.Reset();
    Equipment.Reset();
    Skills.Reset();
    Quests.Reset();
    Social.Reset();
    Notifications.Reset();
    Settings = FAetherUISettings();
}
