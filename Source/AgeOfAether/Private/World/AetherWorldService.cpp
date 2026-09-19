#include "World/AetherWorldService.h"

bool FAetherWorldService::SetConfig(const FAetherWorldConfig& NewConfig)
{
    if (NewConfig.MaxWorldZones < 1 ||
        NewConfig.MaxSpawnsPerZone < 1 ||
        NewConfig.MaxPortals < 1)
    {
        return false;
    }

    Config = NewConfig;
    return true;
}

bool FAetherWorldService::RegisterZone(const FAetherWorldZone& Zone)
{
    if (!Zone.ZoneId.IsValid() ||
        Zone.MinimumLevel < 1 ||
        Zones.Num() >= Config.MaxWorldZones && !Zones.Contains(Zone.ZoneId))
    {
        return false;
    }

    Zones.Add(Zone.ZoneId, Zone);
    return true;
}

bool FAetherWorldService::RegisterSpawnPoint(const FAetherWorldSpawnPoint& SpawnPoint)
{
    if (!SpawnPoint.SpawnId.Len() ||
        !SpawnPoint.ZoneId.IsValid() ||
        SpawnPoint.MinimumLevel < 1 ||
        !Zones.Contains(SpawnPoint.ZoneId))
    {
        return false;
    }

    TArray<FAetherWorldSpawnPoint>& Spawns = SpawnsByZone.FindOrAdd(SpawnPoint.ZoneId);
    if (Spawns.Num() >= Config.MaxSpawnsPerZone)
    {
        return false;
    }

    for (const FAetherWorldSpawnPoint& Existing : Spawns)
    {
        if (Existing.SpawnId == SpawnPoint.SpawnId)
        {
            return false;
        }
    }

    Spawns.Add(SpawnPoint);
    return true;
}

bool FAetherWorldService::RegisterPortal(const FAetherWorldPortal& Portal)
{
    if (!Portal.PortalId.Len() ||
        !Portal.SourceZoneId.IsValid() ||
        !Portal.TargetZoneId.IsValid() ||
        Portal.SourceZoneId == Portal.TargetZoneId ||
        Portal.MinimumLevel < 1 ||
        !Zones.Contains(Portal.SourceZoneId) ||
        !Zones.Contains(Portal.TargetZoneId) ||
        Portals.Num() >= Config.MaxPortals)
    {
        return false;
    }

    for (const FAetherWorldPortal& Existing : Portals)
    {
        if (Existing.PortalId == Portal.PortalId)
        {
            return false;
        }
    }

    Portals.Add(Portal);
    return true;
}

bool FAetherWorldService::FindZone(
    const FAetherWorldZoneId& ZoneId,
    FAetherWorldZone& OutZone) const
{
    const FAetherWorldZone* Zone = Zones.Find(ZoneId);
    if (!Zone)
    {
        return false;
    }

    OutZone = *Zone;
    return true;
}

bool FAetherWorldService::FindSpawnPoint(
    const FAetherWorldZoneId& ZoneId,
    int32 CharacterLevel,
    FAetherWorldSpawnPoint& OutSpawn) const
{
    const TArray<FAetherWorldSpawnPoint>* Spawns = SpawnsByZone.Find(ZoneId);
    if (!Spawns)
    {
        return false;
    }

    const FAetherWorldSpawnPoint* Best = nullptr;
    for (const FAetherWorldSpawnPoint& Spawn : *Spawns)
    {
        if (!Spawn.bEnabled || CharacterLevel < Spawn.MinimumLevel)
        {
            continue;
        }

        if (!Best || Spawn.SpawnId < Best->SpawnId)
        {
            Best = &Spawn;
        }
    }

    if (!Best)
    {
        return false;
    }

    OutSpawn = *Best;
    return true;
}

bool FAetherWorldService::CanEnterZone(
    const FAetherWorldZone& Zone,
    int32 CharacterLevel) const
{
    return Zone.bEnabled && CharacterLevel >= Zone.MinimumLevel;
}

bool FAetherWorldService::ResolveTransition(
    const FAetherWorldZoneId& SourceZoneId,
    const FAetherWorldZoneId& TargetZoneId,
    int32 CharacterLevel,
    FAetherWorldTransitionResult& OutResult) const
{
    OutResult = FAetherWorldTransitionResult();
    OutResult.SourceZoneId = SourceZoneId;
    OutResult.TargetZoneId = TargetZoneId;

    FAetherWorldZone SourceZone;
    FAetherWorldZone TargetZone;
    if (!FindZone(SourceZoneId, SourceZone))
    {
        OutResult.Result = EAetherWorldTransitionResult::ZoneNotFound;
        return false;
    }

    if (!FindZone(TargetZoneId, TargetZone))
    {
        OutResult.Result = EAetherWorldTransitionResult::ZoneNotFound;
        return false;
    }

    if (!SourceZone.bEnabled)
    {
        OutResult.Result = EAetherWorldTransitionResult::InvalidSourceZone;
        return false;
    }

    if (!CanEnterZone(TargetZone, CharacterLevel))
    {
        OutResult.Result = TargetZone.bEnabled
            ? EAetherWorldTransitionResult::LevelRequirement
            : EAetherWorldTransitionResult::ZoneDisabled;
        return false;
    }

    const FAetherWorldPortal* MatchedPortal = nullptr;
    for (const FAetherWorldPortal& Portal : Portals)
    {
        if (Portal.SourceZoneId == SourceZoneId &&
            Portal.TargetZoneId == TargetZoneId)
        {
            MatchedPortal = &Portal;
            break;
        }
    }

    if (!MatchedPortal)
    {
        OutResult.Result = EAetherWorldTransitionResult::PortalNotFound;
        return false;
    }

    if (!MatchedPortal->bEnabled)
    {
        OutResult.Result = EAetherWorldTransitionResult::PortalDisabled;
        return false;
    }

    if (CharacterLevel < MatchedPortal->MinimumLevel)
    {
        OutResult.Result = EAetherWorldTransitionResult::LevelRequirement;
        return false;
    }

    FAetherWorldSpawnPoint Spawn;
    if (!FindSpawnPoint(TargetZoneId, CharacterLevel, Spawn))
    {
        OutResult.Result = EAetherWorldTransitionResult::InvalidSpawn;
        return false;
    }

    OutResult.Result = EAetherWorldTransitionResult::Accepted;
    OutResult.TargetLocation = Spawn.Location;
    OutResult.TargetRotation = Spawn.Rotation;
    OutResult.PortalId = MatchedPortal->PortalId;
    return true;
}

bool FAetherWorldService::InitializeCharacterSpawn(
    const FAetherAccountId& AccountId,
    FAetherCharacterRecord& Character,
    FAetherWorldTransitionResult& OutResult) const
{
    OutResult = FAetherWorldTransitionResult();

    if (!AccountId.IsValid() ||
        Character.AccountId != AccountId ||
        !Character.CharacterId.IsValid() ||
        Character.Status != EAetherCharacterStatus::Active)
    {
        OutResult.Result = EAetherWorldTransitionResult::InvalidRequest;
        return false;
    }

    if (Zones.Num() == 0)
    {
        OutResult.Result = EAetherWorldTransitionResult::ZoneNotFound;
        return false;
    }

    TArray<FAetherWorldZone> CandidateZones;
    for (const TPair<FAetherWorldZoneId, FAetherWorldZone>& Pair : Zones)
    {
        if (CanEnterZone(Pair.Value, Character.Level))
        {
            CandidateZones.Add(Pair.Value);
        }
    }

    CandidateZones.Sort([](const FAetherWorldZone& A, const FAetherWorldZone& B)
    {
        return A.ZoneId.Value < B.ZoneId.Value;
    });

    for (const FAetherWorldZone& Zone : CandidateZones)
    {
        FAetherWorldSpawnPoint Spawn;
        if (FindSpawnPoint(Zone.ZoneId, Character.Level, Spawn))
        {
            Character.WorldLocation = Spawn.Location;
            Character.WorldRotation = Spawn.Rotation;
            Character.CurrentZoneId = Zone.ZoneId;
            Character.CurrentZoneType = Zone.Type;
            OutResult.Result = EAetherWorldTransitionResult::Accepted;
            OutResult.TargetZoneId = Zone.ZoneId;
            OutResult.TargetLocation = Spawn.Location;
            OutResult.TargetRotation = Spawn.Rotation;
            return true;
        }
    }

    OutResult.Result = EAetherWorldTransitionResult::InvalidSpawn;
    return false;
}

bool FAetherWorldService::TransitionCharacter(
    const FAetherAccountId& AccountId,
    FAetherCharacterRecord& Character,
    const FAetherWorldZoneId& TargetZoneId,
    FAetherWorldTransitionResult& OutResult) const
{
    if (!AccountId.IsValid() ||
        Character.AccountId != AccountId ||
        !Character.CharacterId.IsValid() ||
        Character.Status != EAetherCharacterStatus::Active ||
        !Character.CurrentZoneId.IsValid())
    {
        OutResult = FAetherWorldTransitionResult();
        OutResult.Result = EAetherWorldTransitionResult::InvalidRequest;
        return false;
    }

    if (!ResolveTransition(Character.CurrentZoneId, TargetZoneId, Character.Level, OutResult))
    {
        return false;
    }

    FAetherWorldZone TargetZone;
    if (!FindZone(TargetZoneId, TargetZone))
    {
        OutResult.Result = EAetherWorldTransitionResult::ZoneNotFound;
        return false;
    }

    Character.WorldLocation = OutResult.TargetLocation;
    Character.WorldRotation = OutResult.TargetRotation;
    Character.CurrentZoneId = TargetZoneId;
    Character.CurrentZoneType = TargetZone.Type;
    return true;
}
