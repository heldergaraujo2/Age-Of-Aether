#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Networking/AetherNetworkTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProtocolVersionTest,
    "AgeOfAether.Networking.ProtocolVersion",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProtocolVersionTest::RunTest(const FString& Parameters)
{
    const FAetherProtocolVersion Current = FAetherProtocolVersion::Current();

    TestEqual(TEXT("Current protocol major version"), Current.Major, static_cast<uint16>(1));
    TestEqual(TEXT("Current protocol minor version"), Current.Minor, static_cast<uint16>(0));

    FAetherProtocolVersion Compatible = Current;
    Compatible.Minor = 7;

    FAetherProtocolVersion Incompatible = Current;
    Incompatible.Major = 2;

    TestTrue(TEXT("Same major version is compatible"), Current.IsCompatibleWith(Compatible));
    TestTrue(TEXT("Compatibility is symmetric for same major"), Compatible.IsCompatibleWith(Current));
    TestFalse(TEXT("Different major versions are incompatible"), Current.IsCompatibleWith(Incompatible));
    TestEqual(TEXT("Protocol string is stable"), Current.ToString(), FString(TEXT("1.0")));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherNetworkRequestContractTest,
    "AgeOfAether.Networking.RequestResponseContracts",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherNetworkRequestContractTest::RunTest(const FString& Parameters)
{
    FAetherNetworkRequest Request;
    Request.RequestId = 42;
    Request.Type = EAetherNetworkRequestType::QueryAuthoritativeState;
    Request.ProtocolVersion = FAetherProtocolVersion::Current();

    FAetherNetworkResponse Response;
    Response.RequestId = Request.RequestId;
    Response.Result = EAetherNetworkResultCode::Accepted;
    Response.ProtocolVersion = FAetherProtocolVersion::Current();
    Response.AuthoritativeStateRevision = 99;

    TestEqual(TEXT("Request ID is preserved"), Response.RequestId, Request.RequestId);
    TestEqual(TEXT("Accepted response code is preserved"), Response.Result, EAetherNetworkResultCode::Accepted);
    TestTrue(TEXT("Request protocol is compatible"), Request.ProtocolVersion.IsCompatibleWith(Response.ProtocolVersion));
    TestEqual(TEXT("Authoritative revision is preserved"), Response.AuthoritativeStateRevision, static_cast<uint64>(99));

    return true;
}

#endif
