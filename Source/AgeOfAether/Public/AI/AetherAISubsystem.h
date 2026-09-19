#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AI/AetherAIService.h"
#include "AetherAISubsystem.generated.h"
UCLASS()class AGEOFAETHER_API UAetherAISubsystem:public UGameInstanceSubsystem{GENERATED_BODY()public:virtual void Initialize(FSubsystemCollectionBase&)override;virtual void Deinitialize()override;bool InitializeAI(const FAetherAIConfig&,TSharedPtr<IAetherAIProvider>);void ShutdownAI();FAetherAIService&GetService(){return Service;}private:FAetherAIService Service;};
