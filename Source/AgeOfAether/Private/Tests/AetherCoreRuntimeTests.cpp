#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Core/Runtime/AetherRuntimeTypes.h"
#include "Core/Runtime/AetherScheduler.h"
#include "Core/Runtime/AetherServerClock.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherRuntimeTypesTest,
    "AgeOfAether.Core.Runtime.Types",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherRuntimeTypesTest::RunTest(const FString& Parameters)
{
    const FAetherRuntimeResult Success = FAetherRuntimeResult::Success(TEXT("ok"));
    TestTrue(TEXT("Success result is successful"), Success.bSucceeded);
    TestEqual(TEXT("Success result has no error"), Success.Error, EAetherRuntimeError::None);

    const FAetherRuntimeResult Failure = FAetherRuntimeResult::Failure(
        EAetherRuntimeError::InvalidConfiguration,
        TEXT("invalid"));

    TestFalse(TEXT("Failure result is unsuccessful"), Failure.bSucceeded);
    TestEqual(TEXT("Failure result preserves error"), Failure.Error, EAetherRuntimeError::InvalidConfiguration);
    TestEqual(TEXT("Failure result preserves message"), Failure.Message, FString(TEXT("invalid")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherSchedulerTest,
    "AgeOfAether.Core.Runtime.Scheduler",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherSchedulerTest::RunTest(const FString& Parameters)
{
    FAetherScheduler Scheduler;
    int32 OnceCount = 0;
    int32 RepeatingCount = 0;

    const uint64 OnceId = Scheduler.ScheduleOnce(
        1.0,
        FAetherScheduledCallback::CreateLambda([&OnceCount]()
        {
            ++OnceCount;
        }));

    TestNotEqual(TEXT("One-shot task receives an ID"), OnceId, static_cast<uint64>(0));

    Scheduler.Tick(0.5);
    TestEqual(TEXT("One-shot task does not fire early"), OnceCount, 0);

    Scheduler.Tick(1.0);
    TestEqual(TEXT("One-shot task fires at due time"), OnceCount, 1);
    TestEqual(TEXT("One-shot task is removed after execution"), Scheduler.NumTasks(), 0);

    const uint64 RepeatingId = Scheduler.ScheduleRepeating(
        1.0,
        1.0,
        FAetherScheduledCallback::CreateLambda([&RepeatingCount]()
        {
            ++RepeatingCount;
        }));

    TestNotEqual(TEXT("Repeating task receives an ID"), RepeatingId, static_cast<uint64>(0));

    Scheduler.Tick(1.5);
    TestEqual(TEXT("Repeating task does not fire before initial delay"), RepeatingCount, 0);

    Scheduler.Tick(2.0);
    TestEqual(TEXT("Repeating task fires at initial due time"), RepeatingCount, 1);

    Scheduler.Tick(3.0);
    TestEqual(TEXT("Repeating task fires again after interval"), RepeatingCount, 2);

    TestTrue(TEXT("Repeating task can be cancelled"), Scheduler.Cancel(RepeatingId));
    TestEqual(TEXT("Cancelled task is removed"), Scheduler.NumTasks(), 0);

    TestEqual(
        TEXT("Invalid one-shot delay is rejected"),
        Scheduler.ScheduleOnce(-1.0, FAetherScheduledCallback()),
        static_cast<uint64>(0));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherServerClockTest,
    "AgeOfAether.Core.Runtime.ServerClock",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherServerClockTest::RunTest(const FString& Parameters)
{
    FAetherServerClock Clock;

    TestFalse(TEXT("Clock starts stopped"), Clock.IsStarted());
    TestEqual(TEXT("Stopped clock reports zero uptime"), Clock.MonotonicSeconds(), 0.0);

    Clock.Start();
    TestTrue(TEXT("Clock starts"), Clock.IsStarted());
    TestTrue(TEXT("Started clock reports non-negative uptime"), Clock.MonotonicSeconds() >= 0.0);
    TestTrue(TEXT("Clock exposes a UTC timestamp"), Clock.UtcNow().GetTicks() > 0);

    Clock.Stop();
    TestFalse(TEXT("Clock stops"), Clock.IsStarted());
    TestEqual(TEXT("Stopped clock resets uptime"), Clock.MonotonicSeconds(), 0.0);

    return true;
}

#endif
