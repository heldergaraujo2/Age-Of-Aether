#include "Characters/AetherEquipmentVisualProfile.h"
#include "Materials/MaterialInterface.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "PaperSprite.h"
bool UAetherEquipmentVisualProfile::ValidateProfile(FString& OutError) const
{
    OutError.Reset();
    if (VisualProfileID.IsNone() || ItemDefinitionID.IsNone()) { OutError = TEXT("VisualProfileID and ItemDefinitionID are required."); return false; }
    if (EquipmentSlot == EAetherDataEquipmentSlot::None || VisualType == EAetherEquipmentVisualType::None) { OutError = TEXT("EquipmentSlot and VisualType are required."); return false; }
    if (VisualType == EAetherEquipmentVisualType::SkeletalMesh && SkeletalMesh.IsNull()) { OutError = TEXT("SkeletalMesh is required."); return false; }
    if (VisualType == EAetherEquipmentVisualType::StaticMesh && StaticMesh.IsNull()) { OutError = TEXT("StaticMesh is required."); return false; }
    if (VisualType == EAetherEquipmentVisualType::PaperSprite && Sprite.IsNull()) { OutError = TEXT("Sprite is required."); return false; }
    if (VisualType == EAetherEquipmentVisualType::PaperSprite && (SpriteScale.X <= 0.0f || SpriteScale.Y <= 0.0f)) { OutError = TEXT("SpriteScale must be positive."); return false; }
    for (const TSoftObjectPtr<UMaterialInterface>& Material : MaterialOverrides) if (Material.IsNull()) { OutError = TEXT("MaterialOverrides cannot contain null references."); return false; }
    return true;
}