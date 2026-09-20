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

    static UAetherAssetManager* Fallback = nullptr;
    if (!Fallback)
    {
        Fallback = NewObject<UAetherAssetManager>(GetTransientPackage(), TEXT("AetherFallbackAssetManager"));
        Fallback->AddToRoot();
    }

    return *Fallback;
}

void UAetherAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();
}
