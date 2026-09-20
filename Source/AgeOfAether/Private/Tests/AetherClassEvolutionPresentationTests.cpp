#include "Misc/AutomationTest.h"
#include "Characters/AetherClassEvolutionPresentationCatalog.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassEvolutionPresentationCatalogTest,"AgeOfAether.Phase43.ClassEvolutionPresentation.CanonicalCatalog",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassEvolutionPresentationCatalogTest::RunTest(const FString&)
{
 auto* Catalog=NewObject<UAetherClassEvolutionPresentationCatalog>();FString Error;
 TestTrue(TEXT("Catalog validates"),Catalog->ValidateCatalog(Error));TestEqual(TEXT("25 canonical entries"),Catalog->Entries.Num(),25);
 const auto* Archer=Catalog->Find(TEXT("archer"),TEXT("archer.05"));TestNotNull(TEXT("Archer 05 resolves"),Archer);
 if(Archer){TestEqual(TEXT("Archer 05 name"),Archer->DisplayName.ToString(),FString(TEXT("Olho de Deus / Juiz dos Ventos")));TestEqual(TEXT("Archer path"),Archer->PathTitle.ToString(),FString(TEXT("Caminho da Precisão")));}
 const auto* Mage=Catalog->FindWithFallback(TEXT("mage"),TEXT("mage.05"));TestNotNull(TEXT("Mage fallback"),Mage);if(Mage)TestEqual(TEXT("Mage fallback stage 1"),Mage->EvolutionID,FName(TEXT("mage.01")));
 TestNull(TEXT("Unknown class rejected"),Catalog->FindWithFallback(TEXT("unknown"),TEXT("unknown.01")));return true;
}