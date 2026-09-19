#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "World/AetherWorldService.h"

#include "AetherWorldSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherWorldSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool Configure(const FAetherWorldConfig& NewConfig);
    bool RegisterZone(const FAetherWorldZone& Zone);
    bool RegisterSpawnPoint(const FAetherWorldSpawnPoint& SpawnPoint);
    bool RegisterPortal(const FAetherWorldPortal& Portal);

    bool InitializeCharacterSpawn(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        FAetherWorldTransitionResult& OutResult);

    bool TransitionCharacter(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        const FAetherWorldZoneId& TargetZoneId,
        FAetherWorldTransitionResult& OutResult);

    bool FindZone(const FAetherWorldZoneId& ZoneId, FAetherWorldZone& OutZone) const;
    bool FindSpawnPoint(const FAetherWorldZoneId& ZoneId, int32 CharacterLevel, FAetherWorldSpawnPoint& OutSpawn) const;

    bool ConfigureFromDataAsset(const class UAetherWorldConfigDataAsset* ConfigAsset);

private:
    FAetherWorldService WorldService;
};
