#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Multiplayer/AetherMultiplayerService.h"
#include "Multiplayer/AetherMultiplayerSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherMultiplayerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    bool RegisterConnection(uint32 ConnectionId, double NowSeconds);
    bool UnregisterConnection(uint32 ConnectionId);
    bool SetAuthenticated(uint32 ConnectionId, bool bAuthenticated);
    bool Heartbeat(uint32 ConnectionId, double NowSeconds);
    EAetherAuthorityResult ConsumeRequest(uint32 ConnectionId, double NowSeconds);
    int32 RemoveTimedOutConnections(double NowSeconds);
    int32 GetConnectionCount() const;
    int32 GetAuthenticatedCount() const;
    const FAetherMultiplayerConfig& GetConfig() const;
    FAetherMultiplayerService& GetService() { return Service; }
private:
    FAetherMultiplayerService Service;
};