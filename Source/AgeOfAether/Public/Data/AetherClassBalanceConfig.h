#pragma once
#include "CoreMinimal.h"
#include "Data/AetherClassBalanceRegistry.h"
class FAetherClassBalanceConfigLoader
{
public:
 static bool Parse(const FString& Text,FAetherBalanceConfig& OutConfig,FString& OutError);
 static FString Serialize(const FAetherBalanceConfig& Config);
 static bool BuildRegistry(const FAetherBalanceConfig& Config,FAetherClassBalanceRegistry& OutRegistry,FString& OutError);
};
