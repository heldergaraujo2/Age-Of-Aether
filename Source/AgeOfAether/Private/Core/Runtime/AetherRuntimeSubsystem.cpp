#include "Core/Runtime/AetherRuntimeSubsystem.h"

#include "Core/Runtime/AetherCoreLog.h"
#include "Containers/Ticker.h"

void UAetherRuntimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (RuntimeState != EAetherRuntimeState::Uninitialized)
    {
        UE_LOG(LogAgeOfAetherCore, Error, TEXT("Runtime initialization attempted from invalid state."));
        RuntimeState = EAetherRuntimeState::Failed;
        HealthState = EAetherHealthState::Unhealthy;
        return;
    }

    RuntimeState = EAetherRuntimeState::Initializing;
    ServerClock.Start();
    TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateUObject(this, &UAetherRuntimeSubsystem::TickRuntime),
        0.0f);

    RuntimeState = EAetherRuntimeState::Running;
    HealthState = EAetherHealthState::Healthy;

    UE_LOG(LogAgeOfAetherCore, Log, TEXT("Age of Aether core runtime initialized."));
}

void UAetherRuntimeSubsystem::Deinitialize()
{
    if (TickerHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
        TickerHandle.Reset();
    }

    RuntimeState = EAetherRuntimeState::ShuttingDown;
    ServiceRegistry.ShutdownAll();
    Scheduler.Clear();
    ServerClock.Stop();
    HealthState = EAetherHealthState::Unknown;
    RuntimeState = EAetherRuntimeState::Stopped;

    UE_LOG(LogAgeOfAetherCore, Log, TEXT("Age of Aether core runtime stopped."));
    Super::Deinitialize();
}

EAetherRuntimeState UAetherRuntimeSubsystem::GetRuntimeState() const
{
    return RuntimeState;
}

EAetherHealthState UAetherRuntimeSubsystem::GetHealthState() const
{
    return HealthState;
}

double UAetherRuntimeSubsystem::GetServerUptimeSeconds() const
{
    return ServerClock.MonotonicSeconds();
}

FAetherRuntimeResult UAetherRuntimeSubsystem::RequestShutdown()
{
    if (RuntimeState != EAetherRuntimeState::Running)
    {
        return FAetherRuntimeResult::Failure(
            EAetherRuntimeError::InvalidTransition,
            TEXT("Runtime is not running."));
    }

    RuntimeState = EAetherRuntimeState::ShuttingDown;
    HealthState = EAetherHealthState::Degraded;
    return FAetherRuntimeResult::Success(TEXT("Shutdown requested."));
}

FAetherServerClock& UAetherRuntimeSubsystem::GetServerClock()
{
    return ServerClock;
}

FAetherScheduler& UAetherRuntimeSubsystem::GetScheduler()
{
    return Scheduler;
}

bool UAetherRuntimeSubsystem::TickRuntime(float DeltaTime)
{
    if (RuntimeState != EAetherRuntimeState::Running)
    {
        return true;
    }

    Scheduler.Tick(ServerClock.MonotonicSeconds());
    return true;
}

FAetherRuntimeResult UAetherRuntimeSubsystem::TransitionTo(EAetherRuntimeState NewState)
{
    if (RuntimeState == NewState)
    {
        return FAetherRuntimeResult::Success();
    }

    RuntimeState = NewState;
    return FAetherRuntimeResult::Success();
}

void UAetherRuntimeSubsystem::SetHealth(EAetherHealthState NewHealth)
{
    HealthState = NewHealth;
}
