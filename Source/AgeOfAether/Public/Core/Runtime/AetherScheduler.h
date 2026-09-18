#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE(FAetherScheduledCallback);

struct FAetherScheduledTask
{
    uint64 TaskId = 0;
    double DueAtSeconds = 0.0;
    FAetherScheduledCallback Callback;
    bool bRepeating = false;
    double IntervalSeconds = 0.0;
};

class AGEOFAETHER_API FAetherScheduler
{
public:
    uint64 ScheduleOnce(double DelaySeconds, FAetherScheduledCallback Callback);
    uint64 ScheduleRepeating(double InitialDelaySeconds, double IntervalSeconds, FAetherScheduledCallback Callback);

    bool Cancel(uint64 TaskId);
    void Tick(double CurrentSeconds);
    void Clear();

    int32 NumTasks() const;

private:
    uint64 NextTaskId = 1;
    TArray<FAetherScheduledTask> Tasks;
};
