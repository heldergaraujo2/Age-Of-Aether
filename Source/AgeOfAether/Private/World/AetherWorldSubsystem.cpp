#include "World/AetherWorldSubsystem.h"

#include "Characters/AetherCharacterSubsystem.h"
#include "Engine/GameInstance.h"
#include "World/AetherWorldConfigDataAsset.h"

bool UAetherWorldSubsystem::Configure(const FAetherWorldConfig& NewConfig)
{
    return WorldService.SetConfig(NewConfig);
}

bool UAetherWorldSubsystem::RegisterZone(const FAetherWorldZone& Zone)
{
    return WorldService.RegisterZone(Zone);
}

bool UAetherWorldSubsystem::RegisterSpawnPoint(const FAetherWorldSpawnPoint& SpawnPoint)
{
    return WorldService.RegisterSpawnPoint(SpawnPoint);
}

bool UAetherWorldSubsystem::RegisterPortal(const FAetherWorldPortal& Portal)
{
    return WorldService.RegisterPortal(Portal);
}

bool UAetherWorldSubsystem::InitializeCharacterSpawn(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    FAetherWorldTransitionResult& OutResult)
{
    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (!Characters)
    {
        OutResult.Result = EAetherWorldTransitionResult::InvalidRequest;
        return false;
    }

    FAetherCharacterRecord Character;
    if (!Characters->FindCharacter(CharacterId, Character))
    {
        OutResult.Result = EAetherWorldTransitionResult::CharacterNotSelected;
        return false;
    }

    if (!WorldService.InitializeCharacterSpawn(AccountId, Character, OutResult))
    {
        return false;
    }

    return Characters->UpdateCharacterWorldState(
        AccountId,
        CharacterId,
        Character.CurrentZoneId,
        Character.CurrentZoneType,
        Character.WorldLocation,
        Character.WorldRotation);
}

bool UAetherWorldSubsystem::TransitionCharacter(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    const FAetherWorldZoneId& TargetZoneId,
    FAetherWorldTransitionResult& OutResult)
{
    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (!Characters)
    {
        OutResult.Result = EAetherWorldTransitionResult::InvalidRequest;
        return false;
    }

    FAetherCharacterRecord Character;
    if (!Characters->FindCharacter(CharacterId, Character))
    {
        OutResult.Result = EAetherWorldTransitionResult::CharacterNotSelected;
        return false;
    }

    if (!WorldService.TransitionCharacter(AccountId, Character, TargetZoneId, OutResult))
    {
        return false;
    }

    return Characters->UpdateCharacterWorldState(
        AccountId,
        CharacterId,
        Character.CurrentZoneId,
        Character.CurrentZoneType,
        Character.WorldLocation,
        Character.WorldRotation);
}

bool UAetherWorldSubsystem::FindZone(
    const FAetherWorldZoneId& ZoneId,
    FAetherWorldZone& OutZone) const
{
    return WorldService.FindZone(ZoneId, OutZone);
}

bool UAetherWorldSubsystem::FindSpawnPoint(
    const FAetherWorldZoneId& ZoneId,
    int32 CharacterLevel,
    FAetherWorldSpawnPoint& OutSpawn) const
{
    return WorldService.FindSpawnPoint(ZoneId, CharacterLevel, OutSpawn);
}

bool UAetherWorldSubsystem::ConfigureFromDataAsset(
    const UAetherWorldConfigDataAsset* ConfigAsset)
{
    if (!ConfigAsset || !WorldService.SetConfig(ConfigAsset->Config))
    {
        return false;
    }

    WorldService.ClearWorldDefinitions();

    for (const FAetherWorldZone& Zone : ConfigAsset->Zones)
    {
        if (!WorldService.RegisterZone(Zone))
        {
            WorldService.ClearWorldDefinitions();
            return false;
        }
    }

    for (const FAetherWorldSpawnPoint& Spawn : ConfigAsset->SpawnPoints)
    {
        if (!WorldService.RegisterSpawnPoint(Spawn))
        {
            WorldService.ClearWorldDefinitions();
            return false;
        }
    }

    for (const FAetherWorldPortal& Portal : ConfigAsset->Portals)
    {
        if (!WorldService.RegisterPortal(Portal))
        {
            WorldService.ClearWorldDefinitions();
            return false;
        }
    }

    return true;
}
