#include "Characters/AetherCharacterSubsystem.h"

bool UAetherCharacterSubsystem::CreateCharacter(
    const FAetherAccountId& AccountId,
    const FString& Name,
    EAetherCharacterClass CharacterClass,
    FAetherCharacterRecord& OutCharacter)
{
    return CharacterService.CreateCharacter(AccountId, Name, CharacterClass, OutCharacter);
}

bool UAetherCharacterSubsystem::FindCharacter(
    const FAetherCharacterId& CharacterId,
    FAetherCharacterRecord& OutCharacter) const
{
    return CharacterService.FindCharacter(CharacterId, OutCharacter);
}

bool UAetherCharacterSubsystem::SelectCharacter(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    FAetherCharacterRecord& OutCharacter)
{
    return CharacterService.SelectCharacter(AccountId, CharacterId, OutCharacter);
}

bool UAetherCharacterSubsystem::DeselectCharacter(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId)
{
    return CharacterService.DeselectCharacter(AccountId, CharacterId);
}

bool UAetherCharacterSubsystem::UpdateCharacterLocation(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    const FVector& Location,
    const FRotator& Rotation)
{
    return CharacterService.UpdateCharacterLocation(AccountId, CharacterId, Location, Rotation);
}

bool UAetherCharacterSubsystem::GetSelectedCharacter(
    const FAetherAccountId& AccountId,
    FAetherCharacterRecord& OutCharacter) const
{
    return CharacterService.GetSelectedCharacter(AccountId, OutCharacter);
}

int32 UAetherCharacterSubsystem::NumCharacters() const
{
    return CharacterService.NumCharacters();
}

int32 UAetherCharacterSubsystem::NumCharactersForAccount(const FAetherAccountId& AccountId) const
{
    return CharacterService.NumCharactersForAccount(AccountId);
}
