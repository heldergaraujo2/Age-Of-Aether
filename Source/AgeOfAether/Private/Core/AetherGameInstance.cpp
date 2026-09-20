#include "Core/AetherGameInstance.h"

#include "Engine/World.h"

UAetherGameInstance::UAetherGameInstance()
{
}

UAetherGameInstance* UAetherGameInstance::GetAether(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    if (UWorld* World = WorldContextObject->GetWorld())
    {
        return Cast<UAetherGameInstance>(World->GetGameInstance());
    }

    return nullptr;
}

FName UAetherGameInstance::GetDevelopmentMapId() const
{
    return DevelopmentMapId;
}

int32 UAetherGameInstance::GetVisualFoundationVersion() const
{
    return VisualFoundationVersion;
}
