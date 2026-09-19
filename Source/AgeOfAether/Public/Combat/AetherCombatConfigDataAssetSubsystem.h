#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Combat/AetherCombatConfigDataAsset.h"

#include "AetherCombatConfigDataAssetSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherCombatConfigDataAssetSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Intentionally empty: the asset is an authoring contract. Runtime injection is exposed by UAetherCombatSubsystem::Configure.
};
