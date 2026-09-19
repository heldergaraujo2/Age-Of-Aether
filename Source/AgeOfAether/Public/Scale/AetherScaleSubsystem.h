#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Scale/AetherScaleService.h"
#include "AetherScaleSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherScaleSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    FAetherScaleService& GetService() { return Service; }
    const FAetherScaleService& GetService() const { return Service; }
    bool InitializeServer(const FAetherScaleConfig& Config, const FAetherServerNodeId& NodeId, double NowSeconds);
    void ShutdownServer();
private:
    FAetherScaleService Service;
};
