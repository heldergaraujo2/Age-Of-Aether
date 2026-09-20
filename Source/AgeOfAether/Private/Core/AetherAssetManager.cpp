#include "Core/AetherAssetManager.h"

#include "Engine/Engine.h"

UAetherAssetManager& UAetherAssetManager::Get()
{
    if (GEngine)
    {
        if (UAetherAssetManager* Manager = Cast<UAetherAssetManager>(GEngine->AssetManager))
        {
            return *Manager;
        }
    }

    return Fallback;
}

void UAetherAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();
}

static UAetherAssetManager Fallback;
