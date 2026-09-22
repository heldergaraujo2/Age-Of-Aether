#include "Misc/AutomationTest.h"
#include "Scale/AetherScaleService.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleConfigTest, "AgeOfAether.Scale.Config", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleConfigTest::RunTest(const FString&)
{
    FAetherScaleConfig Config;
    TestTrue(TEXT("Default config is valid"), Config.IsValid());
    Config.MaxPlayers = 0;
    TestFalse(TEXT("Zero max players rejected"), Config.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleAdmissionTest, "AgeOfAether.Scale.Admission", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleAdmissionTest::RunTest(const FString&)
{
    FAetherScaleService Service;
    FAetherScaleConfig Config;
    Config.MaxPlayers = 2; Config.MaxConnectionsPerShard = 2;
    FAetherServerNodeId NodeId; NodeId.Value = TEXT("shard-1");
    TestTrue(TEXT("Initialize"), Service.Initialize(Config, NodeId, 0.0));
    TestEqual(TEXT("First accepted"), Service.RegisterPlayer(1.0), EAetherScaleResult::Accepted);
    TestEqual(TEXT("Second accepted"), Service.RegisterPlayer(2.0), EAetherScaleResult::Accepted);
    TestEqual(TEXT("Third rejected"), Service.RegisterPlayer(3.0), EAetherScaleResult::ServerFull);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleDrainTest, "AgeOfAether.Scale.Drain", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleDrainTest::RunTest(const FString&)
{
    FAetherScaleService Service; FAetherScaleConfig Config; FAetherServerNodeId NodeId; NodeId.Value = TEXT("shard-1");
    Service.Initialize(Config, NodeId, 0.0);
    TestEqual(TEXT("Drain accepted"), Service.BeginDrain(), EAetherScaleResult::Accepted);
    TestEqual(TEXT("Admission rejected"), Service.RegisterPlayer(1.0), EAetherScaleResult::ServerDraining);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleHeartbeatTest, "AgeOfAether.Scale.Heartbeat", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleHeartbeatTest::RunTest(const FString&)
{
    FAetherScaleService Service; FAetherScaleConfig Config; FAetherServerNodeId NodeId; NodeId.Value = TEXT("shard-1");
    Service.Initialize(Config, NodeId, 0.0);
    FAetherServerLoadSnapshot Load; Load.MaxPlayers = Config.MaxConnectionsPerShard; Load.ActiveConnections = 5; Load.AuthenticatedPlayers = 5;
    TestEqual(TEXT("Load accepted"), Service.UpdateLoad(Load, 10.0), EAetherScaleResult::Accepted);
    TestTrue(TEXT("Healthy"), Service.IsHealthy(20.0));
    TestFalse(TEXT("Stale node unhealthy"), Service.IsHealthy(25.1));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleHardLoadTest, "AgeOfAether.Scale.HardLoad", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleHardLoadTest::RunTest(const FString&)
{
    FAetherScaleService Service; FAetherScaleConfig Config; FAetherServerNodeId NodeId; NodeId.Value = TEXT("shard-1");
    Service.Initialize(Config, NodeId, 0.0);
    FAetherServerLoadSnapshot Load; Load.MaxPlayers = 100; Load.ActiveConnections = 100; Load.AuthenticatedPlayers = 100;
    Config.MaxPlayers = 100; Config.MaxConnectionsPerShard = 100;
    Service.Shutdown();
    Service.Initialize(Config, NodeId, 0.0);
    TestEqual(TEXT("Load accepted"), Service.UpdateLoad(Load, 1.0), EAetherScaleResult::Accepted);
    TestTrue(TEXT("Hard load drains"), Service.IsDraining());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleTransferTest, "AgeOfAether.Scale.Transfer", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleTransferTest::RunTest(const FString&)
{
    FAetherScaleService Service; FAetherScaleConfig Config; Config.TransferTimeoutSeconds = 5.0;
    FAetherServerNodeId Source; Source.Value = TEXT("source"); FAetherServerNodeId Destination; Destination.Value = TEXT("destination");
    Service.Initialize(Config, Source, 10.0);
    FAetherServerTransferRequest Request;
    TestEqual(TEXT("Transfer created"), Service.CreateTransferRequest(TEXT("account"), TEXT("character"), Destination, TEXT("zone"), 10.0, Request), EAetherScaleResult::Accepted);
    TestEqual(TEXT("Transfer valid"), Service.ValidateTransferRequest(Request, 12.0), EAetherScaleResult::Accepted);
    TestEqual(TEXT("Transfer expires"), Service.ValidateTransferRequest(Request, 16.0), EAetherScaleResult::TransferExpired);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleValidationTest, "AgeOfAether.Scale.Validation", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherScaleValidationTest::RunTest(const FString&)
{
    FAetherScaleService Service; FAetherScaleConfig Config; FAetherServerNodeId Source; Source.Value = TEXT("source");
    FAetherServerNodeId Destination; Destination.Value = TEXT("source"); Service.Initialize(Config, Source, 0.0);
    FAetherServerTransferRequest Request;
    TestEqual(TEXT("Self transfer rejected"), Service.CreateTransferRequest(TEXT("a"), TEXT("c"), Destination, TEXT("z"), 1.0, Request), EAetherScaleResult::InvalidTransfer);
    return true;
}
