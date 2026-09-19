#pragma once
#include "CoreMinimal.h"
#include "Scale/AetherScaleTypes.h"

class FAetherScaleService
{
public:
    bool Initialize(const FAetherScaleConfig& InConfig, const FAetherServerNodeId& InNodeId, double NowSeconds);
    void Shutdown();
    EAetherScaleResult RegisterPlayer(double NowSeconds);
    bool UnregisterPlayer();
    EAetherScaleResult UpdateLoad(const FAetherServerLoadSnapshot& InSnapshot, double NowSeconds);
    EAetherScaleResult BeginDrain();
    EAetherScaleResult ResumeAccepting();
    bool CanAcceptConnection() const;
    bool IsHealthy(double NowSeconds) const;
    bool IsDraining() const;
    float GetLoadScore() const;
    const FAetherScaleConfig& GetConfig() const { return Config; }
    const FAetherServerNodeDescriptor& GetNode() const { return Node; }
    const FAetherServerLoadSnapshot& GetLoad() const { return Load; }
    EAetherScaleResult CreateTransferRequest(const FString& AccountId, const FString& CharacterId,
        const FAetherServerNodeId& DestinationNode, const FString& TargetZoneId, double NowSeconds,
        FAetherServerTransferRequest& OutRequest);
    EAetherScaleResult ValidateTransferRequest(const FAetherServerTransferRequest& Request, double NowSeconds) const;
private:
    FAetherScaleConfig Config;
    FAetherServerNodeDescriptor Node;
    FAetherServerLoadSnapshot Load;
    uint64 NextTransferId = 1;
    bool bRunning = false;
};
