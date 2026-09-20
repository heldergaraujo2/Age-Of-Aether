#pragma once
#include "CoreMinimal.h"
#include "Data/AetherClassTypes.h"
class FAetherClassRegistry;
class FAetherClassCatalog
{
public:
 static void BuildBaseClasses(TArray<FAetherClassDefinition>& OutClasses);
 static void BuildEvolutions(TArray<FAetherClassEvolutionDefinition>& OutEvolutions);
 static bool BuildRegistry(FAetherClassRegistry& Registry, FString& OutError);
};
