#pragma once

#include "CoreMinimal.h"
#include "Accounts/AetherAccountSessionTypes.h"
#include "Characters/AetherCharacterTypes.h"
#include "World/AetherWorldTypes.h"

class FAetherWorldService
{
public:
    bool SetConfig(const FAetherWorldConfig& NewConfig);

    bool RegisterZone(const FAetherWorldZone& Zone);
    bool RegisterSpawnPoint(const FAetherWorldSpawnPoint& SpawnPoint);
    bool RegisterPortal(const FAetherWorldPortal& Portal);
    void ClearWorldDefinitions();

    bool FindZone(const FAetherWorldZoneId& ZoneId, FAetherWorldZone& OutZone) const;
    bool FindSpawnPoint(const FAetherWorldZoneId& ZoneId, int32 CharacterLevel, FAetherWorldSpawnPoint& OutSpawn) const;
    bool CanEnterZone(const FAetherWorldZone& Zone, int32 CharacterLevel) const;

    bool ResolveTransition(
        const FAetherWorldZoneId& SourceZoneId,
        const FAetherWorldZoneId& TargetZoneId,
        int32 CharacterLevel,
        FAetherWorldTransitionResult& OutResult) const;

    bool InitializeCharacterSpawn(
        const FAetherAccountId& AccountId,
        FAetherCharacterRecord& Character,
        FAetherWorldTransitionResult& OutResult) const;

    bool TransitionCharacter(
        const FAetherAccountId& AccountId,
        FAetherCharacterRecord& Character,
        const FAetherWorldZoneId& TargetZoneId,
        FAetherWorldTransitionResult& OutResult) const;

    const FAetherWorldConfig& GetConfig() const { return Config; }

private:
    FAetherWorldConfig Config;
    TMap<FAetherWorldZoneId, FAetherWorldZone> Zones;
    TMap<FAetherWorldZoneId, TArray<FAetherWorldSpawnPoint>> SpawnsByZone;
    TArray<FAetherWorldPortal> Portals;
};
