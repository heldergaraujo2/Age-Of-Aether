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
