#include "Core/Runtime/AetherServerClock.h"

#include "HAL/PlatformTime.h"

void FAetherServerClock::Start()
{
    StartMonotonicSeconds = FPlatformTime::Seconds();
    bStarted = true;
}

void FAetherServerClock::Stop()
{
    bStarted = false;
    StartMonotonicSeconds = 0.0;
}

bool FAetherServerClock::IsStarted() const
{
    return bStarted;
}

FDateTime FAetherServerClock::UtcNow() const
{
    return FDateTime::UtcNow();
}

double FAetherServerClock::MonotonicSeconds() const
{
    if (!bStarted)
    {
        return 0.0;
    }

    return FPlatformTime::Seconds() - StartMonotonicSeconds;
}
