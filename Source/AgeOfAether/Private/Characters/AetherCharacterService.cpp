#include "Characters/AetherCharacterService.h"

bool FAetherCharacterService::CreateCharacter(
    const FAetherAccountId& AccountId,
    const FString& Name,
    EAetherCharacterClass CharacterClass,
    FAetherCharacterRecord& OutCharacter)
{
    if (!AccountId.IsValid())
    {
        return false;
    }

    const FString NormalizedName = NormalizeName(Name);
    if (NormalizedName.IsEmpty() || NormalizedName.Len() < 3 || NormalizedName.Len() > 16)
    {
        return false;
    }

    if (CharacterIdByName.Contains(NormalizedName) || NumCharactersForAccount(AccountId) >= MaxCharactersPerAccount)
    {
        return false;
    }

    FAetherCharacterRecord Character;
    Character.CharacterId = FAetherCharacterId::NewId();
    Character.AccountId = AccountId;
    Character.Name = NormalizedName;
    Character.CharacterClass = CharacterClass;
    switch (CharacterClass)
    {
    case EAetherCharacterClass::Mage: Character.ClassID = TEXT("mage"); Character.EvolutionID = TEXT("mage.01"); break;
    case EAetherCharacterClass::Archer: Character.ClassID = TEXT("archer"); Character.EvolutionID = TEXT("archer.01"); break;
    case EAetherCharacterClass::Cleric: Character.ClassID = TEXT("healer"); Character.EvolutionID = TEXT("healer.01"); break;
    case EAetherCharacterClass::Warrior:
    default: Character.ClassID = TEXT("warrior"); Character.EvolutionID = TEXT("warrior.01"); break;
    }
    Character.Status = EAetherCharacterStatus::Available;
    Character.Level = 1;
    Character.Experience = 0;
    Character.BaseStats = FAetherBaseStats();
    Character.DerivedStats = CalculateDerivedStats(Character.BaseStats, Character.Level);
    Character.CurrentHealth = Character.DerivedStats.MaxHealth;
    Character.CurrentShield = 0.0f;
    Character.CombatState = EAetherCharacterCombatState::Alive;

    Characters.Add(Character.CharacterId, Character);
    CharacterIdByName.Add(NormalizedName, Character.CharacterId);
    CharacterIdsByAccount.FindOrAdd(AccountId).Add(Character.CharacterId);
    OutCharacter = Character;
    return true;
}

bool FAetherCharacterService::RestoreCharacter(const FAetherCharacterRecord& PersistedCharacter)
{
    if (!PersistedCharacter.CharacterId.IsValid()
        || !PersistedCharacter.AccountId.IsValid()
        || PersistedCharacter.Name.TrimStartAndEnd().Len() < 3
        || PersistedCharacter.Name.TrimStartAndEnd().Len() > 16
        || PersistedCharacter.Level < 1
        || PersistedCharacter.Experience < 0
        || PersistedCharacter.UnspentStatPoints < 0)
    {
        return false;
    }

    const FString NormalizedName = NormalizeName(PersistedCharacter.Name);
    if (const FAetherCharacterId* ExistingByName = CharacterIdByName.Find(NormalizedName))
    {
        if (*ExistingByName != PersistedCharacter.CharacterId)
        {
            return false;
        }
    }

    if (FAetherCharacterRecord* Existing = Characters.Find(PersistedCharacter.CharacterId))
    {
        if (Existing->AccountId != PersistedCharacter.AccountId)
        {
            return false;
        }

        CharacterIdByName.Remove(NormalizeName(Existing->Name));
        Existing->Name = NormalizedName;
        Existing->Status = EAetherCharacterStatus::Offline;
        Existing->CurrentHealth = FMath::Clamp(PersistedCharacter.CurrentHealth, 0.0f, PersistedCharacter.DerivedStats.MaxHealth);
        Existing->CurrentShield = FMath::Max(0.0f, PersistedCharacter.CurrentShield);
        Existing->CombatState = PersistedCharacter.CombatState;
        Existing->Level = PersistedCharacter.Level;
        Existing->CharacterClass = PersistedCharacter.CharacterClass;
        Existing->ClassID = PersistedCharacter.ClassID.IsEmpty() ? TEXT("warrior") : PersistedCharacter.ClassID.ToLower();
        Existing->EvolutionID = PersistedCharacter.EvolutionID.IsEmpty() ? Existing->ClassID + TEXT(".01") : PersistedCharacter.EvolutionID.ToLower();
        Existing->Experience = PersistedCharacter.Experience;
        Existing->UnspentStatPoints = PersistedCharacter.UnspentStatPoints;
        Existing->BaseStats = PersistedCharacter.BaseStats;
        Existing->DerivedStats = PersistedCharacter.DerivedStats;
        Existing->WorldLocation = PersistedCharacter.WorldLocation;
        Existing->WorldRotation = PersistedCharacter.WorldRotation;
        Existing->CurrentZoneId = PersistedCharacter.CurrentZoneId;
        Existing->CurrentZoneType = PersistedCharacter.CurrentZoneType;
        CharacterIdByName.Add(Existing->Name, Existing->CharacterId);
        SelectedCharacterByAccount.Remove(Existing->AccountId);
        return true;
    }

    if (NumCharactersForAccount(PersistedCharacter.AccountId) >= MaxCharactersPerAccount)
    {
        return false;
    }

    FAetherCharacterRecord Restored = PersistedCharacter;
    Restored.Name = NormalizedName;
    Restored.ClassID = Restored.ClassID.IsEmpty() ? TEXT("warrior") : Restored.ClassID.ToLower();
    Restored.EvolutionID = Restored.EvolutionID.IsEmpty() ? Restored.ClassID + TEXT(".01") : Restored.EvolutionID.ToLower();
    Restored.Status = EAetherCharacterStatus::Offline;
    Characters.Add(Restored.CharacterId, Restored);
    CharacterIdByName.Add(Restored.Name, Restored.CharacterId);
    CharacterIdsByAccount.FindOrAdd(Restored.AccountId).Add(Restored.CharacterId);
    SelectedCharacterByAccount.Remove(Restored.AccountId);
    return true;
}

bool FAetherCharacterService::FindCharacter(
    const FAetherCharacterId& CharacterId,
    FAetherCharacterRecord& OutCharacter) const
{
    const FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character)
    {
        return false;
    }

    OutCharacter = *Character;
    return true;
}

void FAetherCharacterService::GetCharactersForAccount(
    const FAetherAccountId& AccountId,
    TArray<FAetherCharacterRecord>& OutCharacters) const
{
    OutCharacters.Reset();

    const TSet<FAetherCharacterId>* CharacterIds = CharacterIdsByAccount.Find(AccountId);
    if (!CharacterIds)
    {
        return;
    }

    for (const FAetherCharacterId& CharacterId : *CharacterIds)
    {
        if (const FAetherCharacterRecord* Character = Characters.Find(CharacterId))
        {
            if (Character->Status != EAetherCharacterStatus::Deleted)
            {
                OutCharacters.Add(*Character);
            }
        }
    }

    OutCharacters.Sort([](const FAetherCharacterRecord& A, const FAetherCharacterRecord& B)
    {
        return A.Name < B.Name;
    });
}

bool FAetherCharacterService::FindCharacterByName(
    const FString& Name,
    FAetherCharacterRecord& OutCharacter) const
{
    const FAetherCharacterId* CharacterId = CharacterIdByName.Find(NormalizeName(Name));
    return CharacterId && FindCharacter(*CharacterId, OutCharacter);
}

bool FAetherCharacterService::IsCharacterOwnedByAccount(
    const FAetherCharacterId& CharacterId,
    const FAetherAccountId& AccountId) const
{
    const FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    return Character && Character->AccountId == AccountId;
}

bool FAetherCharacterService::SelectCharacter(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    FAetherCharacterRecord& OutCharacter)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character || Character->AccountId != AccountId)
    {
        return false;
    }

    if (Character->Status == EAetherCharacterStatus::Disabled ||
        Character->Status == EAetherCharacterStatus::Deleted)
    {
        return false;
    }

    if (const FAetherCharacterId* Existing = SelectedCharacterByAccount.Find(AccountId))
    {
        if (*Existing != CharacterId)
        {
            return false;
        }
    }

    Character->Status = EAetherCharacterStatus::Active;
    SelectedCharacterByAccount.Add(AccountId, CharacterId);
    OutCharacter = *Character;
    return true;
}

bool FAetherCharacterService::DeselectCharacter(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character || Character->AccountId != AccountId)
    {
        return false;
    }

    const FAetherCharacterId* Selected = SelectedCharacterByAccount.Find(AccountId);
    if (!Selected || *Selected != CharacterId)
    {
        return false;
    }

    SelectedCharacterByAccount.Remove(AccountId);
    Character->Status = EAetherCharacterStatus::Offline;
    return true;
}

bool FAetherCharacterService::UpdateCharacterLocation(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    const FVector& Location,
    const FRotator& Rotation)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character || Character->AccountId != AccountId || Character->Status != EAetherCharacterStatus::Active)
    {
        return false;
    }

    Character->WorldLocation = Location;
    Character->WorldRotation = Rotation;
    return true;
}

bool FAetherCharacterService::UpdateCharacterWorldState(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    const FAetherWorldZoneId& ZoneId,
    EAetherWorldZoneType ZoneType,
    const FVector& Location,
    const FRotator& Rotation)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character ||
        Character->AccountId != AccountId ||
        Character->Status != EAetherCharacterStatus::Active ||
        !ZoneId.IsValid())
    {
        return false;
    }

    Character->WorldLocation = Location;
    Character->WorldRotation = Rotation;
    Character->CurrentZoneId = ZoneId;
    Character->CurrentZoneType = ZoneType;
    return true;
}

bool FAetherCharacterService::UpdateCharacterStatus(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    EAetherCharacterStatus Status)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character || Character->AccountId != AccountId)
    {
        return false;
    }

    if (Status == EAetherCharacterStatus::Active &&
        (!SelectedCharacterByAccount.Contains(AccountId) ||
         *SelectedCharacterByAccount.Find(AccountId) != CharacterId))
    {
        return false;
    }

    Character->Status = Status;
    if (Status != EAetherCharacterStatus::Active)
    {
        if (const FAetherCharacterId* Selected = SelectedCharacterByAccount.Find(AccountId))
        {
            if (*Selected == CharacterId)
            {
                SelectedCharacterByAccount.Remove(AccountId);
            }
        }
    }

    return true;
}

bool FAetherCharacterService::ApplyProgressionState(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    const FAetherCharacterRecord& UpdatedCharacter)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character || Character->AccountId != AccountId
        || UpdatedCharacter.CharacterId != CharacterId
        || UpdatedCharacter.AccountId != AccountId)
    {
        return false;
    }

    if (Character->Status == EAetherCharacterStatus::Disabled ||
        Character->Status == EAetherCharacterStatus::Deleted)
    {
        return false;
    }

    Character->Level = UpdatedCharacter.Level;
    Character->Experience = UpdatedCharacter.Experience;
    Character->UnspentStatPoints = UpdatedCharacter.UnspentStatPoints;
    Character->BaseStats = UpdatedCharacter.BaseStats;
    Character->DerivedStats = UpdatedCharacter.DerivedStats;
    Character->CurrentHealth = FMath::Clamp(UpdatedCharacter.CurrentHealth, 0.0f, Character->DerivedStats.MaxHealth);
    Character->CurrentShield = FMath::Max(0.0f, UpdatedCharacter.CurrentShield);
    Character->CombatState = Character->CurrentHealth > 0.0f
        ? EAetherCharacterCombatState::Alive
        : EAetherCharacterCombatState::Dead;
    return true;
}

bool FAetherCharacterService::ApplyCombatState(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    const FAetherCharacterRecord& UpdatedCharacter)
{
    FAetherCharacterRecord* Character = Characters.Find(CharacterId);
    if (!Character || Character->AccountId != AccountId
        || UpdatedCharacter.CharacterId != CharacterId
        || UpdatedCharacter.AccountId != AccountId)
    {
        return false;
    }

    if (Character->Status != EAetherCharacterStatus::Active
        || Character->Status == EAetherCharacterStatus::Disabled
        || Character->Status == EAetherCharacterStatus::Deleted)
    {
        return false;
    }

    Character->CurrentHealth = FMath::Clamp(
        UpdatedCharacter.CurrentHealth,
        0.0f,
        Character->DerivedStats.MaxHealth);
    Character->CurrentShield = FMath::Max(0.0f, UpdatedCharacter.CurrentShield);
    Character->CombatState = Character->CurrentHealth > 0.0f
        ? EAetherCharacterCombatState::Alive
        : EAetherCharacterCombatState::Dead;
    return true;
}

bool FAetherCharacterService::GetSelectedCharacter(
    const FAetherAccountId& AccountId,
    FAetherCharacterRecord& OutCharacter) const
{
    const FAetherCharacterId* CharacterId = SelectedCharacterByAccount.Find(AccountId);
    return CharacterId && FindCharacter(*CharacterId, OutCharacter);
}

int32 FAetherCharacterService::NumCharacters() const
{
    return Characters.Num();
}

int32 FAetherCharacterService::NumCharactersForAccount(const FAetherAccountId& AccountId) const
{
    const TSet<FAetherCharacterId>* CharacterIds = CharacterIdsByAccount.Find(AccountId);
    return CharacterIds ? CharacterIds->Num() : 0;
}

FString FAetherCharacterService::NormalizeName(const FString& Name)
{
    return Name.TrimStartAndEnd().ToLower();
}

FAetherDerivedStats FAetherCharacterService::CalculateDerivedStats(
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
    Stats.Resistance = FMath::Clamp(BaseStats.Energy * 0.1f, 0.0f, 75.0f);
    Stats.MoveSpeed = 600.0f;
    return Stats;
}
