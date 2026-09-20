#include "Core/AetherGameInstance.h"
#include "Core/AetherAssetManager.h"
#include "Data/AetherVisualFoundationDataAsset.h"
#include "Characters/AetherCharacter.h"
#include "UI/AetherFoundationHUD.h"
#include "World/AetherDevelopmentWorldActor.h"

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherVisualFoundationPrimaryAssetIdTest,
    "AgeOfAether.VisualFoundation.PrimaryAssetId",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherVisualFoundationPrimaryAssetIdTest::RunTest(const FString& Parameters)
{
    UAetherVisualFoundationDataAsset* Asset =
        NewObject<UAetherVisualFoundationDataAsset>(GetTransientPackage());

    Asset->AssetID = TEXT("character.base");
    Asset->DefinitionID = TEXT("character");
    Asset->Version = TEXT("1");

    const FPrimaryAssetId PrimaryId = Asset->GetPrimaryAssetId();

    TestEqual(TEXT("Primary asset type"), PrimaryId.PrimaryAssetType, FPrimaryAssetType(TEXT("AetherVisual")));
    TestEqual(TEXT("Primary asset name"), PrimaryId.PrimaryAssetName, FName(TEXT("character.base")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherVisualFoundationCharacterComponentsTest,
    "AgeOfAether.VisualFoundation.CharacterComponents",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherVisualFoundationCharacterComponentsTest::RunTest(const FString& Parameters)
{
    AAetherCharacter* Character =
        NewObject<AAetherCharacter>(GetTransientPackage());

    TestNotNull(TEXT("Camera boom exists"), Character->GetCameraBoom());
    TestNotNull(TEXT("Follow camera exists"), Character->GetFollowCamera());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherVisualFoundationClassesTest,
    "AgeOfAether.VisualFoundation.FoundationClasses",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherVisualFoundationClassesTest::RunTest(const FString& Parameters)
{
    TestTrue(TEXT("GameInstance class is valid"), UAetherGameInstance::StaticClass() != nullptr);
    TestTrue(TEXT("AssetManager class is valid"), UAetherAssetManager::StaticClass() != nullptr);
    TestTrue(TEXT("HUD class is valid"), AAetherFoundationHUD::StaticClass() != nullptr);
    TestTrue(TEXT("Development world actor is valid"), AAetherDevelopmentWorldActor::StaticClass() != nullptr);
    return true;
}
