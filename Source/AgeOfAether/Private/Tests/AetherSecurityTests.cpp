#include "Security/AetherSecurityService.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSecurityConfigTest, "AgeOfAether.Security.Config",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherSecurityConfigTest::RunTest(const FString&)
{
    FAetherSecurityService Service;
    FAetherSecurityConfig Config;
    TestTrue(TEXT("default config valid"), Config.IsValid());
    Config.BurstCapacity = 1;
    Config.RequestsPerSecond = 2;
    TestFalse(TEXT("burst below rate rejected"), Config.IsValid());
    TestFalse(TEXT("invalid config cannot initialize"), Service.Initialize(Config));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSecurityRateLimitTest, "AgeOfAether.Security.RateLimit",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherSecurityRateLimitTest::RunTest(const FString&)
{
    FAetherSecurityService Service;
    FAetherSecurityConfig Config;
    Config.RequestsPerSecond = 2;
    Config.BurstCapacity = 2;
    Service.Initialize(Config);

    TestEqual(TEXT("first accepted"), Service.AuthorizeRequest(1, 1, EAetherSecurityAction::Combat, true, 0.0), EAetherSecurityResult::Accepted);
    TestEqual(TEXT("second accepted"), Service.AuthorizeRequest(1, 2, EAetherSecurityAction::Combat, true, 0.0), EAetherSecurityResult::Accepted);
    TestEqual(TEXT("third rate limited"), Service.AuthorizeRequest(1, 3, EAetherSecurityAction::Combat, true, 0.0), EAetherSecurityResult::RateLimited);
    TestEqual(TEXT("refill after elapsed time"), Service.AuthorizeRequest(1, 4, EAetherSecurityAction::Combat, true, 1.0), EAetherSecurityResult::Accepted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSecurityAuthGateTest, "AgeOfAether.Security.AuthenticationGate",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherSecurityAuthGateTest::RunTest(const FString&)
{
    FAetherSecurityService Service;
    Service.Initialize(FAetherSecurityConfig{});

    TestEqual(TEXT("unauthenticated gameplay rejected"),
        Service.AuthorizeRequest(1, 1, EAetherSecurityAction::Inventory, false, 0.0),
        EAetherSecurityResult::NotAuthenticated);

    TestEqual(TEXT("authentication request allowed"),
        Service.AuthorizeRequest(1, 2, EAetherSecurityAction::Authentication, false, 0.0),
        EAetherSecurityResult::Accepted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSecurityQuarantineTest, "AgeOfAether.Security.Quarantine",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherSecurityQuarantineTest::RunTest(const FString&)
{
    FAetherSecurityService Service;
    FAetherSecurityConfig Config;
    Config.MaxInvalidRequestsBeforeQuarantine = 3;
    Config.QuarantineSeconds = 5.0;
    Service.Initialize(Config);

    Service.RecordInvalidRequest(1, EAetherSecurityAction::Combat, 10, 0.0);
    Service.RecordInvalidRequest(1, EAetherSecurityAction::Combat, 11, 0.1);
    TestEqual(TEXT("third invalid quarantines"),
        Service.RecordInvalidRequest(1, EAetherSecurityAction::Combat, 12, 0.2),
        EAetherSecurityResult::Quarantined);

    TestTrue(TEXT("quarantine active"), Service.IsQuarantined(1, 1.0));
    TestEqual(TEXT("quarantined request rejected"),
        Service.AuthorizeRequest(1, 13, EAetherSecurityAction::Combat, true, 1.0),
        EAetherSecurityResult::Quarantined);
    TestFalse(TEXT("quarantine expires"), Service.IsQuarantined(1, 6.0));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSecurityMovementTest, "AgeOfAether.Security.Movement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherSecurityMovementTest::RunTest(const FString&)
{
    FAetherSecurityService Service;
    FAetherSecurityConfig Config;
    Config.MaxMovementSpeedUnitsPerSecond = 100.0;
    Config.MovementGraceSeconds = 0.0;
    Service.Initialize(Config);

    Service.AuthorizeRequest(1, 1, EAetherSecurityAction::World, true, 0.0);
    TestTrue(TEXT("first movement establishes baseline"), Service.ValidateMovement(1, FVector::ZeroVector, 0.0));
    TestTrue(TEXT("legal movement accepted"), Service.ValidateMovement(1, FVector(50.0, 0.0, 0.0), 0.5));
    TestFalse(TEXT("impossible movement rejected"), Service.ValidateMovement(1, FVector(500.0, 0.0, 0.0), 0.6));
    TestTrue(TEXT("suspicion recorded"), Service.GetSuspicionScore(1) > 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSecurityAuditTest, "AgeOfAether.Security.Audit",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FAetherSecurityAuditTest::RunTest(const FString&)
{
    FAetherSecurityService Service;
    Service.Initialize(FAetherSecurityConfig{});
    Service.AuthorizeRequest(1, 1, EAetherSecurityAction::Economy, true, 0.0);
    Service.RecordSuspiciousEvent(1, EAetherSecurityAction::Economy, 2, 0.1, 3);

    TestEqual(TEXT("audit entries recorded"), Service.GetAuditEvents().Num(), 2);
    TestEqual(TEXT("suspicion severity applied"), Service.GetSuspicionScore(1), 3);
    return true;
}
