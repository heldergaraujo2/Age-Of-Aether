#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Accounts/AetherAccountSessionService.h"

namespace
{
    FAetherAccountRecord MakeAccount(
        const FString& Username,
        int32 Permissions = static_cast<int32>(EAetherPermission::Player))
    {
        FAetherAccountRecord Account;
        Account.AccountId = FAetherAccountId::NewId();
        Account.Username = Username;
        Account.Status = EAetherAccountStatus::Active;
        Account.Permissions = Permissions;
        return Account;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherAccountRegistrationTest,
    "AgeOfAether.Accounts.Registration",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAccountRegistrationTest::RunTest(const FString& Parameters)
{
    FAetherAccountSessionService Service;
    const FAetherAccountRecord Account = MakeAccount(TEXT("  Hero  "));

    TestTrue(TEXT("First account registration succeeds"), Service.RegisterAccount(Account));
    TestFalse(TEXT("Duplicate normalized username is rejected"), Service.RegisterAccount(Account));
    TestEqual(TEXT("Account count is one"), Service.NumAccounts(), 1);

    FAetherAccountRecord Found;
    TestTrue(TEXT("Account lookup is case and whitespace insensitive"),
        Service.FindAccountByUsername(TEXT(" HERO "), Found));
    TestEqual(TEXT("Stored username is normalized"), Found.Username, FString(TEXT("hero")));
    TestEqual(TEXT("Account identity is preserved"), Found.AccountId.Value, Account.AccountId.Value);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherAuthenticationLifecycleTest,
    "AgeOfAether.Accounts.AuthenticationLifecycle",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAuthenticationLifecycleTest::RunTest(const FString& Parameters)
{
    FAetherAccountSessionService Service(10.0);
    const FAetherAccountRecord Account = MakeAccount(
        TEXT("Hero"),
        static_cast<int32>(EAetherPermission::Player) | static_cast<int32>(EAetherPermission::VIP));

    TestTrue(TEXT("Account registration succeeds"), Service.RegisterAccount(Account));

    Service.SetCredentialVerifier([](const FString& Username, const FString& CredentialProof)
    {
        return Username == TEXT("hero") && CredentialProof == TEXT("valid-proof");
    });

    const FAetherProtocolVersion Protocol = FAetherProtocolVersion::Current();

    const FAetherAuthenticationResponse BadCredentials =
        Service.Authenticate(TEXT("Hero"), TEXT("bad-proof"), Protocol, 1.0);
    TestEqual(TEXT("Invalid credentials are rejected"),
        BadCredentials.Result,
        EAetherAuthenticationResult::InvalidCredentials);

    const FAetherAuthenticationResponse Login =
        Service.Authenticate(TEXT(" Hero "), TEXT("valid-proof"), Protocol, 2.0);
    TestEqual(TEXT("Valid login is accepted"), Login.Result, EAetherAuthenticationResult::Accepted);
    TestTrue(TEXT("Login returns account ID"), Login.AccountId.IsValid());
    TestTrue(TEXT("Login returns session ID"), Login.SessionId.IsValid());
    TestEqual(TEXT("Permissions are returned"),
        Login.Permissions,
        static_cast<int32>(EAetherPermission::Player) | static_cast<int32>(EAetherPermission::VIP));
    TestEqual(TEXT("One session exists"), Service.NumSessions(), 1);

    const FAetherAuthenticationResponse DuplicateLogin =
        Service.Authenticate(TEXT("hero"), TEXT("valid-proof"), Protocol, 3.0);
    TestEqual(TEXT("Second simultaneous login is rejected"),
        DuplicateLogin.Result,
        EAetherAuthenticationResult::AlreadyOnline);

    TestTrue(TEXT("Heartbeat refreshes active session"),
        Service.Heartbeat(Login.SessionId, Protocol, 8.0));

    FAetherSessionRecord Session;
    TestTrue(TEXT("Session can be inspected"),
        Service.FindSession(Login.SessionId, Session));
    TestEqual(TEXT("Heartbeat keeps session active"), Session.State, EAetherSessionState::Active);
    TestEqual(TEXT("Heartbeat timestamp is updated"), Session.LastHeartbeatAtSeconds, 8.0);

    const FAetherAuthenticationResponse Reconnect = Service.Reconnect(
        Login.AccountId,
        Login.SessionId,
        TEXT("valid-proof"),
        Protocol,
        9.0);
    TestEqual(TEXT("Reconnect is accepted"), Reconnect.Result, EAetherAuthenticationResult::Accepted);

    TestTrue(TEXT("Logout closes the session"),
        Service.Logout(Login.SessionId, Protocol, 9.5));
    TestFalse(TEXT("Closed session no longer accepts heartbeat"),
        Service.Heartbeat(Login.SessionId, Protocol, 10.0));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherSessionTimeoutTest,
    "AgeOfAether.Accounts.SessionTimeout",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherSessionTimeoutTest::RunTest(const FString& Parameters)
{
    FAetherAccountSessionService Service(5.0);
    const FAetherAccountRecord Account = MakeAccount(TEXT("TimeoutUser"));

    TestTrue(TEXT("Timeout account registration succeeds"), Service.RegisterAccount(Account));
    Service.SetCredentialVerifier([](const FString&, const FString& CredentialProof)
    {
        return CredentialProof == TEXT("valid-proof");
    });

    const FAetherAuthenticationResponse Login = Service.Authenticate(
        TEXT("timeoutuser"),
        TEXT("valid-proof"),
        FAetherProtocolVersion::Current(),
        100.0);

    TestEqual(TEXT("Timeout test login succeeds"), Login.Result, EAetherAuthenticationResult::Accepted);
    TestEqual(TEXT("Session does not expire before timeout"),
        Service.ExpireTimedOutSessions(105.0), 0);
    TestEqual(TEXT("Session expires after timeout"),
        Service.ExpireTimedOutSessions(105.1), 1);

    FAetherSessionRecord Session;
    TestTrue(TEXT("Expired session remains inspectable for diagnostics"),
        Service.FindSession(Login.SessionId, Session));
    TestEqual(TEXT("Expired session state is TimedOut"),
        Session.State,
        EAetherSessionState::TimedOut);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherAccountStatusTest,
    "AgeOfAether.Accounts.Status",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAccountStatusTest::RunTest(const FString& Parameters)
{
    FAetherAccountSessionService Service;
    const FAetherAccountRecord Account = MakeAccount(TEXT("StatusUser"));

    TestTrue(TEXT("Status account registration succeeds"), Service.RegisterAccount(Account));
    Service.SetCredentialVerifier([](const FString&, const FString& CredentialProof)
    {
        return CredentialProof == TEXT("valid-proof");
    });

    TestTrue(TEXT("Account can be disabled"),
        Service.UpdateAccountStatus(Account.AccountId, EAetherAccountStatus::Disabled));

    const FAetherAuthenticationResponse Disabled = Service.Authenticate(
        TEXT("statususer"),
        TEXT("valid-proof"),
        FAetherProtocolVersion::Current(),
        1.0);
    TestEqual(TEXT("Disabled account cannot authenticate"),
        Disabled.Result,
        EAetherAuthenticationResult::AccountDisabled);

    TestTrue(TEXT("Account can be re-enabled"),
        Service.UpdateAccountStatus(Account.AccountId, EAetherAccountStatus::Active));

    const FAetherAuthenticationResponse Enabled = Service.Authenticate(
        TEXT("statususer"),
        TEXT("valid-proof"),
        FAetherProtocolVersion::Current(),
        2.0);
    TestEqual(TEXT("Re-enabled account can authenticate"),
        Enabled.Result,
        EAetherAuthenticationResult::Accepted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherAuthenticationProtocolTest,
    "AgeOfAether.Accounts.ProtocolValidation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAuthenticationProtocolTest::RunTest(const FString& Parameters)
{
    FAetherAccountSessionService Service;
    const FAetherAccountRecord Account = MakeAccount(TEXT("ProtocolUser"));

    TestTrue(TEXT("Protocol account registration succeeds"), Service.RegisterAccount(Account));
    Service.SetCredentialVerifier([](const FString&, const FString&) { return true; });

    FAetherProtocolVersion Incompatible = FAetherProtocolVersion::Current();
    ++Incompatible.Major;

    const FAetherAuthenticationResponse Response = Service.Authenticate(
        TEXT("protocoluser"),
        TEXT("proof"),
        Incompatible,
        1.0);

    TestEqual(TEXT("Incompatible protocol is rejected before authentication"),
        Response.Result,
        EAetherAuthenticationResult::InvalidProtocol);
    TestEqual(TEXT("Protocol rejection creates no session"), Service.NumSessions(), 0);
    return true;
}

#endif
