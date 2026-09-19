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

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Networking")
    uint64 GetAuthoritativeStateRevision() const;
    UFUNCTION(BlueprintPure, Category = "Age of Aether|Networking") int32 GetConnectedPlayerCount() const { return ConnectedPlayerCount; }
    UFUNCTION(BlueprintPure, Category = "Age of Aether|Networking") bool IsAcceptingConnections() const { return bAcceptingConnections; }
    void AdvanceAuthoritativeState();
    void SetConnectedPlayerCount(int32 Count);
    void SetAcceptingConnections(bool bAccepting);

protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Networking")
    FAetherProtocolVersion ProtocolVersion;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Networking")
    uint64 AuthoritativeStateRevision = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Networking")
    int32 ConnectedPlayerCount = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Networking")
    bool bAcceptingConnections = true;
};
