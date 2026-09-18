#pragma once

#include "CoreMinimal.h"

struct AGEOFAETHER_API FAetherRuntimeConfig
{
    bool bEnabled = true;
    bool bFailOnInvalidConfiguration = true;
    float TickIntervalSeconds = 0.0f;

    bool Load(FString& OutError);
};
