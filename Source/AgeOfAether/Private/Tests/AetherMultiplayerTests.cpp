#include "Multiplayer/AetherMultiplayerService.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMultiplayerAdmissionTest, "AgeOfAether.Multiplayer.Admission",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherMultiplayerAdmissionTest::RunTest(const FString&)
{
    FAetherMultiplayerService Service;
    FAetherMultiplayerConfig Config;
    Config.MaxPlayers = 2;
    Config.RequestsPerSecond = 2;
    Config.BurstCapacity = 3;
    TestTrue(TEXT("valid config"), Service.Initialize(Config));
    TestEqual(TEXT("first accepted"), Service.RegisterConnection(1, 0.0), EAetherAdmissionResult::Accepted);
    TestEqual(TEXT("second accepted"), Service.RegisterConnection(2, 0.0), EAetherAdmissionResult::Accepted);
    TestEqual(TEXT("third rejected"), Service.RegisterConnection(3, 0.0), EAetherAdmissionResult::ServerFull);
    TestEqual(TEXT("duplicate rejected"), Service.RegisterConnection(1, 1.0), EAetherAdmissionResult::AlreadyConnected);
    TestTrue(TEXT("remove connection"), Service.UnregisterConnection(1));
    TestEqual(TEXT("slot reusable"), Service.RegisterConnection(3, 1.0), EAetherAdmissionResult::Accepted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMultiplayerRateLimitTest, "AgeOfAether.Multiplayer.RateLimit",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherMultiplayerRateLimitTest::RunTest(const FString&)
{
    FAetherMultiplayerService Service;
    FAetherMultiplayerConfig Config;
    Config.MaxPlayers = 4; Config.RequestsPerSecond = 2; Config.BurstCapacity = 2;
    Service.Initialize(Config);
    Service.RegisterConnection(1, 0.0);
    TestEqual(TEXT("unauthenticated rejected"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::NotAuthenticated);
    Service.SetAuthenticated(1, true);
    TestEqual(TEXT("first accepted"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::Accepted);
    TestEqual(TEXT("second accepted"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::Accepted);
    TestEqual(TEXT("third rate limited"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::RateLimited);
    TestEqual(TEXT("refilled request accepted"), Service.ConsumeRequest(1, 1.0), EAetherAuthorityResult::Accepted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMultiplayerHeartbeatTest, "AgeOfAether.Multiplayer.HeartbeatTimeout",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherMultiplayerHeartbeatTest::RunTest(const FString&)
{
    FAetherMultiplayerService Service;
    FAetherMultiplayerConfig Config; Config.MaxPlayers=2; Config.HeartbeatTimeoutSeconds=5.0;
    Service.Initialize(Config);
    Service.RegisterConnection(1, 0.0);
    Service.RegisterConnection(2, 0.0);
    Service.Heartbeat(2, 4.0);
    TestEqual(TEXT("one timed out"), Service.RemoveTimedOutConnections(6.0), 1);
    TestEqual(TEXT("one remains"), Service.GetConnectionCount(), 1);
    TestTrue(TEXT("remaining connection exists"), Service.FindConnection(2) != nullptr);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMultiplayerLifecycleTest, "AgeOfAether.Multiplayer.Lifecycle",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherMultiplayerLifecycleTest::RunTest(const FString&)
{
    FAetherMultiplayerService Service;
    FAetherMultiplayerConfig Invalid;
    Invalid.MaxPlayers = 0;
    TestFalse(TEXT("invalid config rejected"), Service.Initialize(Invalid));

    FAetherMultiplayerConfig Config;
    Config.MaxPlayers = 3; Config.RequestsPerSecond = 4; Config.BurstCapacity = 4;
    TestTrue(TEXT("valid config accepted"), Service.Initialize(Config));
    TestEqual(TEXT("connection accepted"), Service.RegisterConnection(10, 0.0), EAetherAdmissionResult::Accepted);
    TestTrue(TEXT("authentication state set"), Service.SetAuthenticated(10, true));
    TestTrue(TEXT("heartbeat accepted"), Service.Heartbeat(10, 2.0));
    TestTrue(TEXT("connection exists"), Service.FindConnection(10) != nullptr);
    Service.Shutdown();
    TestFalse(TEXT("shutdown rejects new connection"), Service.RegisterConnection(11, 3.0) == EAetherAdmissionResult::Accepted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMultiplayerRequestRefillTest, "AgeOfAether.Multiplayer.RequestRefill",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherMultiplayerRequestRefillTest::RunTest(const FString&)
{
    FAetherMultiplayerService Service;
    FAetherMultiplayerConfig Config;
    Config.MaxPlayers = 1; Config.RequestsPerSecond = 5; Config.BurstCapacity = 5;
    Service.Initialize(Config);
    Service.RegisterConnection(1, 0.0);
    Service.SetAuthenticated(1, true);
    for (int32 Index = 0; Index < 5; ++Index)
        TestEqual(TEXT("burst request accepted"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::Accepted);
    TestEqual(TEXT("burst exhausted"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::RateLimited);
    TestEqual(TEXT("time refill"), Service.ConsumeRequest(1, 1.0), EAetherAuthorityResult::Accepted);
    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMultiplayerHeartbeatRefillIsolationTest, "AgeOfAether.Multiplayer.HeartbeatCannotRefillRequests",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherMultiplayerHeartbeatRefillIsolationTest::RunTest(const FString&)
{
    FAetherMultiplayerService Service;
    FAetherMultiplayerConfig Config;
    Config.MaxPlayers = 1;
    Config.RequestsPerSecond = 2;
    Config.BurstCapacity = 2;
    Service.Initialize(Config);
    Service.RegisterConnection(1, 0.0);
    Service.SetAuthenticated(1, true);

    TestEqual(TEXT("first request"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::Accepted);
    TestEqual(TEXT("second request"), Service.ConsumeRequest(1, 0.0), EAetherAuthorityResult::Accepted);
    Service.Heartbeat(1, 10.0);
    TestEqual(TEXT("heartbeat does not create request budget"), Service.ConsumeRequest(1, 10.0), EAetherAuthorityResult::RateLimited);
    TestEqual(TEXT("elapsed request time refills"), Service.ConsumeRequest(1, 11.0), EAetherAuthorityResult::Accepted);
    return true;
}
