#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Networking/AetherNetworkTypes.h"

#include "AetherNetworkPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherNetworkResponseEvent, const FAetherNetworkResponse&, Response);

UCLASS()
class AGEOFAETHER_API AAetherNetworkPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AAetherNetworkPlayerController();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Networking")
    void SendNetworkRequest(EAetherNetworkRequestType Type);

    UPROPERTY(BlueprintAssignable, Category = "Age of Aether|Networking")
    FAetherNetworkResponseEvent OnNetworkResponse;

protected:
    UFUNCTION(Server, Reliable)
    void ServerSubmitRequest(const FAetherNetworkRequest& Request);

    UFUNCTION(Client, Reliable)
    void ClientReceiveResponse(const FAetherNetworkResponse& Response);

    virtual void BeginPlay() override;

private:
    bool ValidateRequest(const FAetherNetworkRequest& Request) const;
    uint32 NextRequestId = 1;
};
