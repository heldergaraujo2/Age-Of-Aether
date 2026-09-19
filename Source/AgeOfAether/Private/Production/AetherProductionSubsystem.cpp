#include "Production/AetherProductionSubsystem.h"
void UAetherProductionSubsystem::Initialize(FSubsystemCollectionBase&C){Super::Initialize(C);}
void UAetherProductionSubsystem::Deinitialize(){Service.Shutdown(0.0);Super::Deinitialize();}
