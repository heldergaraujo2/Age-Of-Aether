#include "Scale/AetherScaleSubsystem.h"

void UAetherScaleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UAetherScaleSubsystem::Deinitialize()
{
    Service.Shutdown();
    Super::Deinitialize();
}

bool UAetherScaleSubsystem::InitializeServer(const FAetherScaleConfig& Config, const FAetherServerNodeId& NodeId, double NowSeconds)
{
    return Service.Initialize(Config, NodeId, NowSeconds);
}

void UAetherScaleSubsystem::ShutdownServer()
{
    Service.Shutdown();
}
