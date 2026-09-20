#include "Economy/AetherEconomyTypes.h"
bool FAetherRecipeDefinition::IsValid() const
{
    if (RecipeID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty()) return false;
    if (RequiredLevel < 1 || RequiredLevel > 100000) return false;
    if (!CraftingFee.IsValid() || !FMath::IsFinite(SuccessChance) || SuccessChance < 0.0f || SuccessChance > 1.0f) return false;
    if (Ingredients.IsEmpty() || Outputs.IsEmpty()) return false;
    for (const auto& I : Ingredients) if (!I.IsValid()) return false;
    for (const auto& O : Outputs) if (!O.IsValid()) return false;
    return true;
}