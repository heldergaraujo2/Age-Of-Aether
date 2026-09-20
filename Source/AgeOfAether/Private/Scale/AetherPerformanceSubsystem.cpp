#include "Scale/AetherPerformanceSubsystem.h"
bool UAetherPerformanceSubsystem::Configure(const FAetherPerformanceBudget& InBudget){if(!InBudget.IsValid())return false;Budget=InBudget;return true;}
bool UAetherPerformanceSubsystem::SubmitSnapshot(const FAetherPerformanceSnapshot& S){if(!S.IsValid())return false;LastSnapshot=S;bWithinBudget=S.FrameMilliseconds<=Budget.MaxFrameMilliseconds&&S.GameThreadMilliseconds<=Budget.MaxGameThreadMilliseconds&&S.RenderThreadMilliseconds<=Budget.MaxRenderThreadMilliseconds&&S.StreamingMilliseconds<=Budget.MaxStreamingMilliseconds&&S.ActiveMapCount<=Budget.MaxActiveMapCount&&S.VisibleCreatureCount<=Budget.MaxVisibleCreatureCount;return true;}
void UAetherPerformanceSubsystem::Reset(){LastSnapshot=FAetherPerformanceSnapshot{};bWithinBudget=true;}
