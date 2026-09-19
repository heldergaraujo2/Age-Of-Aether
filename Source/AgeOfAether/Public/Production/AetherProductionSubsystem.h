#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Production/AetherProductionService.h"
#include "AetherProductionSubsystem.generated.h"
UCLASS()class AGEOFAETHER_API UAetherProductionSubsystem:public UGameInstanceSubsystem{GENERATED_BODY()public:virtual void Initialize(FSubsystemCollectionBase&)override;virtual void Deinitialize()override;FAetherProductionService&GetService(){return Service;}private:FAetherProductionService Service;};
