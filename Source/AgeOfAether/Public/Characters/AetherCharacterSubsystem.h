#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Characters/AetherCharacterService.h"

#include "AetherCharacterSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherCharacterSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool CreateCharacter(
        const FAetherAccountId& AccountId,
        const FString& Name,
        EAetherCharacterClass CharacterClass,
        FAetherCharacterRecord& OutCharacter);

    bool FindCharacter(const FAetherCharacterId& CharacterId, FAetherCharacterRecord& OutCharacter) const;
    bool FindCharacterByName(const FString& Name, FAetherCharacterRecord& OutCharacter) const;
    bool IsCharacterOwnedByAccount(const FAetherCharacterId& CharacterId, const FAetherAccountId& AccountId) const;
    void GetCharactersForAccount(const FAetherAccountId& AccountId, TArray<FAetherCharacterRecord>& OutCharacters) const;

    bool SelectCharacter(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        FAetherCharacterRecord& OutCharacter);

    bool UpdateCharacterStatus(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        EAetherCharacterStatus Status);

    bool DeselectCharacter(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId);

    bool UpdateCharacterLocation(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        const FVector& Location,
        const FRotator& Rotation);

    bool GetSelectedCharacter(
        const FAetherAccountId& AccountId,
        FAetherCharacterRecord& OutCharacter) const;

    int32 NumCharacters() const;
    int32 NumCharactersForAccount(const FAetherAccountId& AccountId) const;

private:
    FAetherCharacterService CharacterService;
};
