#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Progression/AetherProgressionService.h"
#include "Progression/AetherProgressionSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherProgressionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool GrantExperience(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        int64 ExperienceAmount,
        FAetherProgressionResult& OutResult);

    bool AllocateStatPoints(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        EAetherCharacterStat Stat,
        int32 Amount,
        FAetherProgressionResult& OutResult);

    int64 ExperienceRequiredForNextLevel(int32 CurrentLevel) const;

    const FAetherProgressionConfig& GetConfig() const;

private:
    FAetherProgressionService ProgressionService;
};
