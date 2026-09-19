#pragma once

#include "CoreMinimal.h"
#include "Accounts/AetherAccountSessionTypes.h"
#include "Characters/AetherCharacterTypes.h"

class FAetherCharacterService
{
public:
    static constexpr int32 MaxCharactersPerAccount = 5;

    bool CreateCharacter(
        const FAetherAccountId& AccountId,
        const FString& Name,
        EAetherCharacterClass CharacterClass,
        FAetherCharacterRecord& OutCharacter);

    bool FindCharacter(const FAetherCharacterId& CharacterId, FAetherCharacterRecord& OutCharacter) const;
    bool FindCharacterByName(const FString& Name, FAetherCharacterRecord& OutCharacter) const;
    void GetCharactersForAccount(const FAetherAccountId& AccountId, TArray<FAetherCharacterRecord>& OutCharacters) const;
    bool IsCharacterOwnedByAccount(const FAetherCharacterId& CharacterId, const FAetherAccountId& AccountId) const;

    bool SelectCharacter(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        FAetherCharacterRecord& OutCharacter);

    bool DeselectCharacter(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId);

    bool UpdateCharacterLocation(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        const FVector& Location,
        const FRotator& Rotation);

    bool UpdateCharacterStatus(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        EAetherCharacterStatus Status);

    bool ApplyProgressionState(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        const FAetherCharacterRecord& UpdatedCharacter);

    bool ApplyCombatState(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        const FAetherCharacterRecord& UpdatedCharacter);

    bool GetSelectedCharacter(
        const FAetherAccountId& AccountId,
        FAetherCharacterRecord& OutCharacter) const;

    int32 NumCharacters() const;
    int32 NumCharactersForAccount(const FAetherAccountId& AccountId) const;

private:
    static FString NormalizeName(const FString& Name);
    static FAetherDerivedStats CalculateDerivedStats(const FAetherBaseStats& BaseStats, int32 Level);

    TMap<FAetherCharacterId, FAetherCharacterRecord> Characters;
    TMap<FString, FAetherCharacterId> CharacterIdByName;
    TMap<FAetherAccountId, TSet<FAetherCharacterId>> CharacterIdsByAccount;
    TMap<FAetherAccountId, FAetherCharacterId> SelectedCharacterByAccount;
};
