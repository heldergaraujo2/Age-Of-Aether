#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Networking/AetherNetworkTypes.h"

#include "AetherNetworkGameState.generated.h"

UCLASS()
class AGEOFAETHER_API AAetherNetworkGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AAetherNetworkGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Networking")
    FAetherProtocolVersion GetProtocolVersion() const;

    uint64 GetAuthoritativeStateRevision() const;
    UFUNCTION(BlueprintPure, Category = "Age of Aether|Networking") int32 GetConnectedPlayerCount() const { return ConnectedPlayerCount; }
    UFUNCTION(BlueprintPure, Category = "Age of Aether|Networking") bool IsAcceptingConnections() const { return bAcceptingConnections; }
    void AdvanceAuthoritativeState();
    void SetConnectedPlayerCount(int32 Count);
    void SetAcceptingConnections(bool bAccepting);

protected:
    UPROPERTY(Replicated)
    FAetherProtocolVersion ProtocolVersion;

    UPROPERTY(Replicated)
    uint64 AuthoritativeStateRevision = 0;

    UPROPERTY(Replicated)
    int32 ConnectedPlayerCount = 0;

    UPROPERTY(Replicated)
    bool bAcceptingConnections = true;
};
