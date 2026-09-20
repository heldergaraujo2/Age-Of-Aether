#include "Misc/AutomationTest.h"
#include "Client/AetherClientSubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClientRequestLifecycleTest,"AgeOfAether.Client.Core.RequestLifecycle",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClientRequestLifecycleTest::RunTest(const FString&)
{
    UAetherClientSubsystem* S=NewObject<UAetherClientSubsystem>(); FString Op=TEXT("Login"); const uint32 ID=S->BeginRequest(Op,10.0,5.0);
    FAetherClientRequestState R; TestTrue(TEXT("request exists"),S->GetRequest(ID,R)); TestEqual(TEXT("pending"),R.State,EAetherClientRequestState::Pending);
    TestFalse(TEXT("not expired"),R.IsExpired(14.9)); TestEqual(TEXT("expired count"),S->ExpireRequests(15.0),1);
    TestTrue(TEXT("expired state"),S->GetRequest(ID,R)); TestEqual(TEXT("timeout"),R.State,EAetherClientRequestState::TimedOut);
    TestFalse(TEXT("cannot complete timed out"),S->CompleteRequest(ID,true)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClientIdentityRevisionTest,"AgeOfAether.Client.Core.IdentityRevision",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClientIdentityRevisionTest::RunTest(const FString&)
{
    UAetherClientSubsystem* S=NewObject<UAetherClientSubsystem>(); FAetherClientIdentitySnapshot I; I.AccountID=TEXT("Account.1"); I.SessionID=TEXT("Session.1");
    S->SetConnectionState(EAetherClientConnectionState::Connected); TestTrue(TEXT("apply revision 10"),S->ApplyIdentity(I,10,100.0));
    TestFalse(TEXT("reject stale revision"),S->ApplyIdentity(I,9,101.0)); TestEqual(TEXT("revision retained"),S->GetSnapshot().ServerRevision,(int64)10);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClientStateResetTest,"AgeOfAether.Client.Core.Reset",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClientStateResetTest::RunTest(const FString&)
{
    UAetherClientSubsystem* S=NewObject<UAetherClientSubsystem>(); S->SetConnectionState(EAetherClientConnectionState::Connected); S->SetScreen(EAetherClientScreen::Gameplay);
    FAetherClientIdentitySnapshot I; I.AccountID=TEXT("A"); I.SessionID=TEXT("S"); S->ApplyIdentity(I,1,1.0);
    const uint32 ID=S->BeginRequest(TEXT("Test"),1.0); TestTrue(TEXT("request"),ID>0); S->ResetClient();
    TestEqual(TEXT("offline"),S->GetConnectionState(),EAetherClientConnectionState::Offline); TestEqual(TEXT("screen none"),S->GetScreen(),EAetherClientScreen::None);
    FAetherClientRequestState Cleared; TestFalse(TEXT("request cleared"),S->GetRequest(ID,Cleared)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClientInvalidIdentityTest,"AgeOfAether.Client.Core.InvalidIdentity",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClientInvalidIdentityTest::RunTest(const FString&)
{
    UAetherClientSubsystem* S=NewObject<UAetherClientSubsystem>(); FAetherClientIdentitySnapshot I; I.SessionID=TEXT("S"); TestFalse(TEXT("session without account rejected"),S->ApplyIdentity(I,1,0));
    I.AccountID=TEXT("A"); I.SessionID.Reset(); TestFalse(TEXT("account without session rejected"),S->ApplyIdentity(I,1,0)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClientStateTransitionTest,"AgeOfAether.Client.Core.StateTransitions",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClientStateTransitionTest::RunTest(const FString&)
{
    UAetherClientSubsystem* S=NewObject<UAetherClientSubsystem>(); S->SetConnectionState(EAetherClientConnectionState::Connecting); TestEqual(TEXT("connecting"),S->GetConnectionState(),EAetherClientConnectionState::Connecting);
    S->SetConnectionState(EAetherClientConnectionState::Connected); TestEqual(TEXT("connected"),S->GetConnectionState(),EAetherClientConnectionState::Connected);
    S->SetConnectionState(EAetherClientConnectionState::Authenticating); TestEqual(TEXT("auth"),S->GetConnectionState(),EAetherClientConnectionState::Authenticating);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClientAssetBindingTest,"AgeOfAether.Client.Core.AssetBinding",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClientAssetBindingTest::RunTest(const FString&)
{
    FAetherClientAssetBinding B; B.AssetID=TEXT("UI.Icon.Sword"); B.RuntimePath=TEXT("/Game/UI/Icon/Sword"); B.bResolved=true;
    TestTrue(TEXT("binding resolved"),B.bResolved); TestEqual(TEXT("stable asset id"),B.AssetID,TEXT("UI.Icon.Sword")); B.Reset();
    TestFalse(TEXT("reset unresolved"),B.bResolved); TestTrue(TEXT("reset id"),B.AssetID.IsEmpty()); TestTrue(TEXT("reset path"),B.RuntimePath.IsEmpty()); return true;
}
