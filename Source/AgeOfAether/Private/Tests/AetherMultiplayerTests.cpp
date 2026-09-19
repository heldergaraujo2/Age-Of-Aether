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