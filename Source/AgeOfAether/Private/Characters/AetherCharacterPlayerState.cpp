#include "Characters/AetherCharacterPlayerState.h"

#include "Net/UnrealNetwork.h"

AAetherCharacterPlayerState::AAetherCharacterPlayerState()
{
    bReplicates = true;
}

void AAetherCharacterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAetherCharacterPlayerState, AccountId);
    DOREPLIFETIME(AAetherCharacterPlayerState, CharacterId);
    DOREPLIFETIME(AAetherCharacterPlayerState, CharacterName);
    DOREPLIFETIME(AAetherCharacterPlayerState, CharacterClass);
    DOREPLIFETIME(AAetherCharacterPlayerState, CharacterLevel);
    DOREPLIFETIME(AAetherCharacterPlayerState, CharacterExperience);
    DOREPLIFETIME(AAetherCharacterPlayerState, UnspentStatPoints);
    DOREPLIFETIME(AAetherCharacterPlayerState, BaseStats);
    DOREPLIFETIME(AAetherCharacterPlayerState, DerivedStats);
    DOREPLIFETIME(AAetherCharacterPlayerState, CurrentHealth);
    DOREPLIFETIME(AAetherCharacterPlayerState, CurrentShield);
    DOREPLIFETIME(AAetherCharacterPlayerState, CombatState);
    DOREPLIFETIME(AAetherCharacterPlayerState, CurrentZoneId);
    DOREPLIFETIME(AAetherCharacterPlayerState, CurrentZoneType);
}

void AAetherCharacterPlayerState::SetCharacterIdentity(const FAetherCharacterRecord& Character)
{
    if (!HasAuthority())
    {
        return;
    }

    AccountId = Character.AccountId;
    CharacterId = Character.CharacterId;
    CharacterName = Character.Name;
    CharacterClass = Character.CharacterClass;
    CharacterLevel = Character.Level;
    CharacterExperience = Character.Experience;
    UnspentStatPoints = Character.UnspentStatPoints;
    BaseStats = Character.BaseStats;
    DerivedStats = Character.DerivedStats;
    CurrentHealth = Character.CurrentHealth;
    CurrentShield = Character.CurrentShield;
    CombatState = Character.CombatState;
    CurrentZoneId = Character.CurrentZoneId;
    CurrentZoneType = Character.CurrentZoneType;
    SetPlayerName(Character.Name);
}

FAetherAccountId AAetherCharacterPlayerState::GetAccountId() const
{
    return AccountId;
}

FAetherCharacterId AAetherCharacterPlayerState::GetCharacterId() const
{
    return CharacterId;
}

FString AAetherCharacterPlayerState::GetCharacterName() const
{
    return CharacterName;
}

EAetherCharacterClass AAetherCharacterPlayerState::GetCharacterClass() const
{
    return CharacterClass;
}

int32 AAetherCharacterPlayerState::GetCharacterLevel() const
{
    return CharacterLevel;
}

int64 AAetherCharacterPlayerState::GetCharacterExperience() const
{
    return CharacterExperience;
}

int32 AAetherCharacterPlayerState::GetUnspentStatPoints() const
{
    return UnspentStatPoints;
}

FAetherBaseStats AAetherCharacterPlayerState::GetBaseStats() const
{
    return BaseStats;
}

FAetherDerivedStats AAetherCharacterPlayerState::GetDerivedStats() const
{
    return DerivedStats;
}

float AAetherCharacterPlayerState::GetCurrentHealth() const
{
    return CurrentHealth;
}

float AAetherCharacterPlayerState::GetCurrentShield() const
{
    return CurrentShield;
}

EAetherCharacterCombatState AAetherCharacterPlayerState::GetCombatState() const
{
    return CombatState;
}

FAetherWorldZoneId AAetherCharacterPlayerState::GetCurrentZoneId() const
{
    return CurrentZoneId;
}

EAetherWorldZoneType AAetherCharacterPlayerState::GetCurrentZoneType() const
{
    return CurrentZoneType;
}
