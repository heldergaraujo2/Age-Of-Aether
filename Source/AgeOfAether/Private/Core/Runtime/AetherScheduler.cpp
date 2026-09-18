#include "Core/Runtime/AetherScheduler.h"

uint64 FAetherScheduler::ScheduleOnce(double DelaySeconds, FAetherScheduledCallback Callback)
{
    if (!Callback.IsBound() || DelaySeconds < 0.0)
    {
        return 0;
    }

    FAetherScheduledTask Task;
    Task.TaskId = NextTaskId++;
    Task.DueAtSeconds = DelaySeconds;
    Task.Callback = MoveTemp(Callback);
    Tasks.Add(MoveTemp(Task));
    return Tasks.Last().TaskId;
}

uint64 FAetherScheduler::ScheduleRepeating(double InitialDelaySeconds, double IntervalSeconds, FAetherScheduledCallback Callback)
{
    if (!Callback.IsBound() || InitialDelaySeconds < 0.0 || IntervalSeconds <= 0.0)
    {
        return 0;
    }

    FAetherScheduledTask Task;
    Task.TaskId = NextTaskId++;
    Task.DueAtSeconds = InitialDelaySeconds;
    Task.Callback = MoveTemp(Callback);
    Task.bRepeating = true;
    Task.IntervalSeconds = IntervalSeconds;
    Tasks.Add(MoveTemp(Task));
    return Tasks.Last().TaskId;
}

bool FAetherScheduler::Cancel(uint64 TaskId)
{
    return Tasks.RemoveAll([TaskId](const FAetherScheduledTask& Task)
    {
        return Task.TaskId == TaskId;
    }) > 0;
}

void FAetherScheduler::Tick(double CurrentSeconds)
{
    for (int32 Index = Tasks.Num() - 1; Index >= 0; --Index)
    {
        FAetherScheduledTask& Task = Tasks[Index];
        if (CurrentSeconds < Task.DueAtSeconds)
        {
            continue;
        }

        const FAetherScheduledCallback Callback = Task.Callback;
        if (Task.bRepeating)
        {
            Task.DueAtSeconds = CurrentSeconds + Task.IntervalSeconds;
        }
        else
        {
            Tasks.RemoveAtSwap(Index);
        }

        if (Callback.IsBound())
        {
            Callback.Execute();
        }
    }
}

void FAetherScheduler::Clear()
{
    Tasks.Reset();
}

int32 FAetherScheduler::NumTasks() const
{
    return Tasks.Num();
}
