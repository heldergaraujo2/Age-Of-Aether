#pragma once

#include "CoreMinimal.h"

class AGEOFAETHER_API FAetherServerClock
{
public:
    void Start();
    void Stop();

    bool IsStarted() const;
    FDateTime UtcNow() const;
    double MonotonicSeconds() const;

private:
    double StartMonotonicSeconds = 0.0;
    bool bStarted = false;
};
