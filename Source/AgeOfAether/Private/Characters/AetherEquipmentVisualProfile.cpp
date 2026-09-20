#include "Characters/AetherEquipmentVisualProfile.h"
#include "Materials/MaterialInterface.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
bool UAetherEquipmentVisualProfile::ValidateProfile(FString& OutError) const
{
    OutError.Reset();
    if (VisualProfileID.IsNone() || ItemDefinitionID.IsNone()) { OutError = TEXT("VisualProfileID and ItemDefinitionID are required."); return false; }
    if (EquipmentSlot == EAetherEquipmentSlot::None || VisualType == EAetherEquipmentVisualType::None) { OutError = TEXT("EquipmentSlot and VisualType are required."); return false; }
    if (VisualType == EAetherEquipmentVisualType::SkeletalMesh && SkeletalMesh.IsNull()) { OutError = TEXT("SkeletalMesh is required."); return false; }
    if (VisualType == EAetherEquipmentVisualType::StaticMesh && StaticMesh.IsNull()) { OutError = TEXT("StaticMesh is required."); return false; }
    for (const TSoftObjectPtr<UMaterialInterface>& Material : MaterialOverrides) if (Material.IsNull()) { OutError = TEXT("MaterialOverrides cannot contain null references."); return false; }
    return true;
}