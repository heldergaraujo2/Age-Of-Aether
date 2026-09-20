#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Presentation/AetherClassPresentationCatalog.h"
#include "Presentation/AetherClassPresentationRegistry.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationCatalogCountTest,"AgeOfAether.ClassPresentation.CatalogCount",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationCatalogCountTest::RunTest(const FString&){TArray<FAetherClassPresentationDefinition>D;FAetherClassPresentationCatalog::Build(D);TestEqual(TEXT("25 evolution presentations"),D.Num(),25);return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationCatalogIdentityTest,"AgeOfAether.ClassPresentation.CatalogIdentity",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationCatalogIdentityTest::RunTest(const FString&){TArray<FAetherClassPresentationDefinition>D;FAetherClassPresentationCatalog::Build(D);FAetherClassPresentationRegistry R;FString E;for(const auto& X:D)TestTrue(TEXT("register presentation"),R.Register(X,E));TArray<FAetherClassPresentationValidationIssue>I;TestTrue(TEXT("catalog validates"),R.Validate(I));TestEqual(TEXT("25 registry entries"),R.Num(),25);return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationCatalogIsolationTest,"AgeOfAether.ClassPresentation.CatalogIsolation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationCatalogIsolationTest::RunTest(const FString&){TArray<FAetherClassPresentationDefinition>A,B;FAetherClassPresentationCatalog::Build(A);FAetherClassPresentationCatalog::Build(B);A[0].MeshAssetID=TEXT("mutated");TestNotEqual(TEXT("independent data"),A[0].MeshAssetID,B[0].MeshAssetID);return true;}
#endif