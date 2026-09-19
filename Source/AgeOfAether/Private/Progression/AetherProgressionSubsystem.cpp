#include "Progression/AetherProgressionSubsystem.h"
#include "Characters/AetherCharacterSubsystem.h"

bool UAetherProgressionSubsystem::GrantExperience(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    int64 ExperienceAmount,
    FAetherProgressionResult& OutResult)
{
    OutResult = FAetherProgressionResult();

    if (!AccountId.IsValid() || !CharacterId.IsValid() || ExperienceAmount <= 0)
    {
        OutResult.Result = EAetherProgressionResult::InvalidRequest;
        return false;
    }

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (!Characters)
    {
        OutResult.Result = EAetherProgressionResult::CharacterNotFound;
        return false;
    }

    FAetherCharacterRecord Character;
    if (!Characters->FindCharacter(CharacterId, Character))
    {
        OutResult.Result = EAetherProgressionResult::CharacterNotFound;
        return false;
    }

    if (Character.AccountId != AccountId)
    {
        OutResult.Result = EAetherProgressionResult::NotOwned;
        return false;
    }

    FAetherCharacterRecord UpdatedCharacter = Character;
    if (!ProgressionService.GrantExperience(UpdatedCharacter, ExperienceAmount, OutResult))
    {
        return false;
    }

    if (!Characters->ApplyProgressionState(AccountId, CharacterId, UpdatedCharacter))
    {
        OutResult.Result = EAetherProgressionResult::InvalidRequest;
        return false;
    }

    return true;
}

bool UAetherProgressionSubsystem::AllocateStatPoints(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    EAetherCharacterStat Stat,
    int32 Amount,
    FAetherProgressionResult& OutResult)
{
    OutResult = FAetherProgressionResult();

    if (!AccountId.IsValid() || !CharacterId.IsValid() || Amount <= 0)
    {
        OutResult.Result = EAetherProgressionResult::InvalidRequest;
        return false;
    }

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (!Characters)
    {
        OutResult.Result = EAetherProgressionResult::CharacterNotFound;
        return false;
    }

    FAetherCharacterRecord Character;
    if (!Characters->FindCharacter(CharacterId, Character))
    {
        OutResult.Result = EAetherProgressionResult::CharacterNotFound;
        return false;
    }

    if (Character.AccountId != AccountId)
    {
        OutResult.Result = EAetherProgressionResult::NotOwned;
        return false;
    }

    FAetherCharacterRecord UpdatedCharacter = Character;
    if (!ProgressionService.AllocateStatPoints(UpdatedCharacter, Stat, Amount, OutResult))
    {
        return false;
    }

    if (!Characters->ApplyProgressionState(AccountId, CharacterId, UpdatedCharacter))
    {
        OutResult.Result = EAetherProgressionResult::InvalidRequest;
        return false;
    }

    return true;
}

int64 UAetherProgressionSubsystem::ExperienceRequiredForNextLevel(int32 CurrentLevel) const
{
    return ProgressionService.ExperienceRequiredForNextLevel(CurrentLevel);
}

const FAetherProgressionConfig& UAetherProgressionSubsystem::GetConfig() const
{
    return ProgressionService.GetConfig();
}

bool UAetherProgressionSubsystem::ConfigureFromDataAsset(const UAetherProgressionConfigDataAsset* ConfigAsset)
{
    return ConfigAsset && ProgressionService.SetConfig(ConfigAsset->Config);
}
