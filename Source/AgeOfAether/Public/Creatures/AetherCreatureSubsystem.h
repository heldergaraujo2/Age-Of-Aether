#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Creatures/AetherCreatureRegistry.h"
#include "AetherCreatureSubsystem.generated.h"

class AAetherCreatureActor;

UCLASS()
class AGEOFAETHER_API UAetherCreatureSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    bool RegisterCreature(const FAetherCreatureDefinition& Definition);
    bool RegisterSpawn(const FAetherCreatureSpawnDefinition& Definition);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Creatures")
    AAetherCreatureActor* SpawnCreature(const FString& SpawnID);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Creatures")
    int32 GetAliveCountForSpawn(const FString& SpawnID) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Creatures")
    void DespawnAllCreatures();

    const FAetherCreatureDefinition* FindCreature(const FString& CreatureID) const;
    bool ValidateRegistry(TArray<FString>& OutErrors) const;
    void ResetRegistry();

private:
    FAetherCreatureRegistry Registry;

    UPROPERTY(Transient)
    TArray<TObjectPtr<AAetherCreatureActor>> SpawnedCreatures;
};
