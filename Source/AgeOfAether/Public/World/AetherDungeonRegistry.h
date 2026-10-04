#pragma once

#include "CoreMinimal.h"
#include "World/AetherDungeonTypes.h"

class FAetherDungeonRegistry
{
public:
    bool RegisterDungeon(const FAetherDungeonDefinition& Definition);
    const FAetherDungeonDefinition* FindDungeon(const FString& DungeonID) const;
    bool Validate(TArray<FString>& OutErrors) const;
    void Reset();
    int32 NumDungeons() const { return Dungeons.Num(); }

private:
    static FString Normalize(const FString& Value)
    {
        return Value.TrimStartAndEnd().ToLower();
    }

    TMap<FString, FAetherDungeonDefinition> Dungeons;
};