#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Persistence/AetherPersistenceTypes.h"
#include "AetherPersistenceSaveGame.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherPersistenceSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    int32 SaveSchemaVersion = AETHER_PERSISTENCE_SCHEMA_VERSION;

    UPROPERTY()
    uint64 SaveRevision = 0;

    UPROPERTY()
    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
};
