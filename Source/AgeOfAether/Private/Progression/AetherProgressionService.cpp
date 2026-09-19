#include "Progression/AetherProgressionService.h"

FAetherProgressionService::FAetherProgressionService()
{
    Config.MaxLevel = 4000;
    Config.BaseExperienceToLevel = 100;
    Config.ExperienceGrowthPerLevel = 1.15f;
    Config.StatPointsPerLevel = 5;
    Config.MaxStatValue = 30000;
}

const FAetherProgressionConfig& FAetherProgressionService::GetConfig() const
{
    return Config;
}

int64 FAetherProgressionService::ExperienceRequiredForNextLevel(int32 CurrentLevel) const
{
    if (CurrentLevel < 1 || CurrentLevel >= Config.MaxLevel)
    {
        return 0;
    }

    const double Exponent = static_cast<double>(CurrentLevel - 1);
    const double RawRequired = static_cast<double>(Config.BaseExperienceToLevel)
        * FMath::Pow(static_cast<double>(Config.ExperienceGrowthPerLevel), Exponent);

    return FMath::Max<int64>(1, static_cast<int64>(FMath::CeilToDouble(RawRequired)));
}

bool FAetherProgressionService::GrantExperience(
    FAetherCharacterRecord& Character,
    int64 ExperienceAmount,
    FAetherProgressionResult& OutResult) const
{
    OutResult = FAetherProgressionResult();
    OutResult.PreviousLevel = Character.Level;
    OutResult.NewLevel = Character.Level;
    OutResult.PreviousExperience = Character.Experience;
    OutResult.NewExperience = Character.Experience;
    OutResult.UnspentStatPoints = Character.UnspentStatPoints;

    if (!Character.CharacterId.IsValid() || ExperienceAmount <= 0)
    {
        OutResult.Result = EAetherProgressionResult::InvalidExperience;
        return false;
    }

    if (Character.Status == EAetherCharacterStatus::Disabled)
    {
        OutResult.Result = EAetherProgressionResult::CharacterDisabled;
        return false;
    }

    if (Character.Status == EAetherCharacterStatus::Deleted)
    {
        OutResult.Result = EAetherProgressionResult::CharacterDeleted;
        return false;
    }

    if (Character.Level >= Config.MaxLevel)
    {
        OutResult.Result = EAetherProgressionResult::MaxLevelReached;
        return false;
    }

    if (ExperienceAmount > MAX_int64 - Character.Experience)
    {
        OutResult.Result = EAetherProgressionResult::InvalidExperience;
        return false;
    }

    int64 NewExperience = Character.Experience + ExperienceAmount;
    int32 NewLevel = Character.Level;
    int32 LevelsGained = 0;

    while (NewLevel < Config.MaxLevel)
    {
        const int64 Required = ExperienceRequiredForNextLevel(NewLevel);
        if (Required <= 0 || NewExperience < Required)
        {
            break;
        }

        NewExperience -= Required;
        ++NewLevel;
        ++LevelsGained;
    }

    Character.Level = NewLevel;
    Character.Experience = NewExperience;
    Character.DerivedStats = CalculateDerivedStats(Character.BaseStats, Character.Level);
    Character.UnspentStatPoints += LevelsGained * Config.StatPointsPerLevel;

    OutResult.Result = EAetherProgressionResult::Accepted;
    OutResult.NewLevel = Character.Level;
    OutResult.NewExperience = Character.Experience;
    OutResult.LevelsGained = LevelsGained;
    OutResult.StatPointsGranted = LevelsGained * Config.StatPointsPerLevel;
    OutResult.UnspentStatPoints = Character.UnspentStatPoints;
    return true;
}

bool FAetherProgressionService::AllocateStatPoints(
    FAetherCharacterRecord& Character,
    EAetherCharacterStat Stat,
    int32 Amount,
    FAetherProgressionResult& OutResult) const
{
    OutResult = FAetherProgressionResult();
    OutResult.PreviousLevel = Character.Level;
    OutResult.NewLevel = Character.Level;
    OutResult.PreviousExperience = Character.Experience;
    OutResult.NewExperience = Character.Experience;
    OutResult.UnspentStatPoints = Character.UnspentStatPoints;

    if (!Character.CharacterId.IsValid() || Amount <= 0)
    {
        OutResult.Result = EAetherProgressionResult::InvalidRequest;
        return false;
    }

    if (!IsStatValid(Stat))
    {
        OutResult.Result = EAetherProgressionResult::InvalidStat;
        return false;
    }

    if (Character.Status == EAetherCharacterStatus::Disabled)
    {
        OutResult.Result = EAetherProgressionResult::CharacterDisabled;
        return false;
    }

    if (Character.Status == EAetherCharacterStatus::Deleted)
    {
        OutResult.Result = EAetherProgressionResult::CharacterDeleted;
        return false;
    }

    if (Amount > Character.UnspentStatPoints)
    {
        OutResult.Result = EAetherProgressionResult::InsufficientStatPoints;
        return false;
    }

    const int32 CurrentValue = GetStatValue(Character.BaseStats, Stat);
    if (CurrentValue > Config.MaxStatValue - Amount)
    {
        OutResult.Result = EAetherProgressionResult::StatCapReached;
        return false;
    }

    SetStatValue(Character.BaseStats, Stat, CurrentValue + Amount);
    Character.UnspentStatPoints -= Amount;
    Character.DerivedStats = CalculateDerivedStats(Character.BaseStats, Character.Level);

    OutResult.Result = EAetherProgressionResult::Accepted;
    OutResult.UnspentStatPoints = Character.UnspentStatPoints;
    return true;
}

int32 FAetherProgressionService::GetStatValue(const FAetherBaseStats& Stats, EAetherCharacterStat Stat)
{
    switch (Stat)
    {
    case EAetherCharacterStat::Strength: return Stats.Strength;
    case EAetherCharacterStat::Agility: return Stats.Agility;
    case EAetherCharacterStat::Vitality: return Stats.Vitality;
    case EAetherCharacterStat::Energy: return Stats.Energy;
    case EAetherCharacterStat::Command: return Stats.Command;
    default: return 0;
    }
}

void FAetherProgressionService::SetStatValue(FAetherBaseStats& Stats, EAetherCharacterStat Stat, int32 Value)
{
    switch (Stat)
    {
    case EAetherCharacterStat::Strength: Stats.Strength = Value; break;
    case EAetherCharacterStat::Agility: Stats.Agility = Value; break;
    case EAetherCharacterStat::Vitality: Stats.Vitality = Value; break;
    case EAetherCharacterStat::Energy: Stats.Energy = Value; break;
    case EAetherCharacterStat::Command: Stats.Command = Value; break;
    default: break;
    }
}

FAetherDerivedStats FAetherProgressionService::CalculateDerivedStats(
    const FAetherBaseStats& BaseStats,
    int32 Level)
{
    FAetherDerivedStats Stats;
    const int32 SafeLevel = FMath::Max(1, Level);
    Stats.MaxHealth = 100.0f + BaseStats.Vitality * 10.0f + SafeLevel * 5.0f;
    Stats.MaxMana = 50.0f + BaseStats.Energy * 5.0f + SafeLevel * 2.0f;
    Stats.AttackMin = 10.0f + BaseStats.Strength * 1.5f + SafeLevel;
    Stats.AttackMax = Stats.AttackMin + 5.0f + BaseStats.Agility * 0.25f;
    Stats.Defense = 5.0f + BaseStats.Agility * 0.5f + SafeLevel * 0.5f;
    Stats.MoveSpeed = 600.0f;
    return Stats;
}

bool FAetherProgressionService::IsStatValid(EAetherCharacterStat Stat)
{
    return Stat >= EAetherCharacterStat::Strength && Stat <= EAetherCharacterStat::Command;
}
