#include "Data/AetherItemDataTypes.h"

namespace
{
bool IsFiniteNonNegative(const double Value)
{
    return FMath::IsFinite(Value) && Value >= 0.0;
}
}

bool FAetherDataItemDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty()) { OutError = TEXT("DefinitionID is required."); return false; }
    if (DisplayName.TrimStartAndEnd().IsEmpty()) { OutError = TEXT("DisplayName is required."); return false; }
    if (RequiredCharacterLevel < 0 || ItemLevel < 0) { OutError = TEXT("Item levels cannot be negative."); return false; }
    if (MaxStack <= 0) { OutError = TEXT("MaxStack must be positive."); return false; }
    if (!IsFiniteNonNegative(Weight)) { OutError = TEXT("Weight must be finite and non-negative."); return false; }
    if (BaseDurability < 0 || MaxDurability < 0 || BaseDurability > MaxDurability) { OutError = TEXT("Invalid durability range."); return false; }
    if (Requirements.CharacterLevel < 0) { OutError = TEXT("Requirement CharacterLevel cannot be negative."); return false; }
    if (Category == EAetherDataItemCategory::Equipment && EquipmentSlot == EAetherDataEquipmentSlot::None) { OutError = TEXT("Equipment items require an equipment slot."); return false; }
    if (Category != EAetherDataItemCategory::Equipment && EquipmentSlot != EAetherDataEquipmentSlot::None) { OutError = TEXT("Non-equipment items cannot define an equipment slot."); return false; }
    if (Economy.BuyValue < 0 || Economy.SellValue < 0) { OutError = TEXT("Economy values cannot be negative."); return false; }
    if (MaxEnhancementLevel < 0 || Enhancements.Num() != MaxEnhancementLevel + 1) { OutError = TEXT("Enhancements must contain exactly levels 0 through MaxEnhancementLevel."); return false; }

    TSet<int32> EnhancementLevels;
    for (const FAetherEnhancementLevel& Enhancement : Enhancements)
    {
        if (Enhancement.Level < 0 || Enhancement.Level > MaxEnhancementLevel || EnhancementLevels.Contains(Enhancement.Level)) { OutError = TEXT("Invalid or duplicate enhancement level."); return false; }
        EnhancementLevels.Add(Enhancement.Level);
        if (!FMath::IsFinite(Enhancement.SuccessChance) || Enhancement.SuccessChance < 0.0 || Enhancement.SuccessChance > 1.0) { OutError = TEXT("Enhancement SuccessChance must be in [0,1]."); return false; }
        if (Enhancement.CurrencyAmount < 0) { OutError = TEXT("Enhancement CurrencyAmount cannot be negative."); return false; }
        for (const FAetherEnhancementMaterial& Material : Enhancement.Materials)
        {
            if (Material.ItemID.TrimStartAndEnd().IsEmpty() || Material.Quantity <= 0) { OutError = TEXT("Enhancement materials require a non-empty ItemID and positive Quantity."); return false; }
        }
        for (const TPair<FString, double>& Pair : Enhancement.StatScaling)
        {
            if (!IsFiniteNonNegative(Pair.Value)) { OutError = FString::Printf(TEXT("Enhancement stat scaling '%s' must be finite and non-negative."), *Pair.Key); return false; }
        }
    }
    for (const TPair<FString, double>& Pair : BaseStats)
    {
        if (Pair.Key.TrimStartAndEnd().IsEmpty() || !FMath::IsFinite(Pair.Value)) { OutError = TEXT("BaseStats require non-empty keys and finite values."); return false; }
    }
    for (const TPair<FString, double>& Pair : Options)
    {
        if (Pair.Key.TrimStartAndEnd().IsEmpty() || !FMath::IsFinite(Pair.Value)) { OutError = TEXT("Options require non-empty keys and finite values."); return false; }
    }
    return true;
}