#include "Core/Runtime/AetherRuntimeConfig.h"

bool FAetherRuntimeConfig::Load(FString& OutError)
{
    OutError.Reset();

    if (!GConfig)
    {
        OutError = TEXT("Global configuration is unavailable.");
        return false;
    }

    GConfig->GetBool(TEXT("AgeOfAether.Runtime"), TEXT("bEnabled"), bEnabled, GGameIni);
    GConfig->GetBool(TEXT("AgeOfAether.Runtime"), TEXT("bFailOnInvalidConfiguration"), bFailOnInvalidConfiguration, GGameIni);
    GConfig->GetFloat(TEXT("AgeOfAether.Runtime"), TEXT("TickIntervalSeconds"), TickIntervalSeconds, GGameIni);

    if (TickIntervalSeconds < 0.0f)
    {
        OutError = TEXT("TickIntervalSeconds cannot be negative.");
        return false;
    }

    return true;
}
