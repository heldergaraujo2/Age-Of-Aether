#pragma once
#include "CoreMinimal.h"
#include "Progression/AetherProgressionTypes.h"

class FAetherProgressionService
{
public:
    FAetherProgressionService();

    const FAetherProgressionConfig& GetConfig() const;
    int64 ExperienceRequiredForNextLevel(int32 CurrentLevel) const;

    bool GrantExperience(FAetherCharacterRecord& Character, int64 ExperienceAmount, FAetherProgressionResult& OutResult) const;
    bool AllocateStatPoints(FAetherCharacterRecord& Character, EAetherCharacterStat Stat, int32 Amount, FAetherProgressionResult& OutResult) const;

private:
    static int32 GetStatValue(const FAetherBaseStats& Stats, EAetherCharacterStat Stat);
    static void SetStatValue(FAetherBaseStats& Stats, EAetherCharacterStat Stat, int32 Value);
    static FAetherDerivedStats CalculateDerivedStats(const FAetherBaseStats& BaseStats, int32 Level);
    static bool IsStatValid(EAetherCharacterStat Stat);

    FAetherProgressionConfig Config;
};
