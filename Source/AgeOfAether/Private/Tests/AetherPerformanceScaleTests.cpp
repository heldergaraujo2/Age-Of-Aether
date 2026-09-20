#include "Scale/AetherPerformanceTypes.h"
#include "Scale/AetherScaleTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPerformanceBudgetTest,"AgeOfAether.Performance.Budget",EAutomationTestFlags::ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherPerformanceBudgetTest::RunTest(const FString&){FAetherPerformanceBudget B;TestTrue(TEXT("Default budget valid"),B.IsValid());B.MaxFrameMilliseconds=1.0f;TestFalse(TEXT("Invalid frame budget rejected"),B.IsValid());return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPerformanceSnapshotTest,"AgeOfAether.Performance.Snapshot",EAutomationTestFlags::ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherPerformanceSnapshotTest::RunTest(const FString&){FAetherPerformanceSnapshot S;TestTrue(TEXT("Default snapshot valid"),S.IsValid());S.FrameMilliseconds=-1;TestFalse(TEXT("Negative metric rejected"),S.IsValid());return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherScaleConfigTest,"AgeOfAether.Scale.Config",EAutomationTestFlags::ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherScaleConfigTest::RunTest(const FString&){FAetherScaleConfig C;TestTrue(TEXT("Default scale config valid"),C.IsValid());C.HardLoadThreshold=0.5f;TestFalse(TEXT("Hard threshold below soft rejected"),C.IsValid());return true;}
