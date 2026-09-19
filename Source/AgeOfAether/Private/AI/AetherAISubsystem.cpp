#include "AI/AetherAISubsystem.h"
void UAetherAISubsystem::Initialize(FSubsystemCollectionBase&C){Super::Initialize(C);}
void UAetherAISubsystem::Deinitialize(){Service.Shutdown();Super::Deinitialize();}
bool UAetherAISubsystem::InitializeAI(const FAetherAIConfig&C,TSharedPtr<IAetherAIProvider>P){return Service.Initialize(C,MoveTemp(P));}
void UAetherAISubsystem::ShutdownAI(){Service.Shutdown();}
