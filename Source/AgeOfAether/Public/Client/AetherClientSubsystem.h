#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Client/AetherClientTypes.h"
#include "AetherClientSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherClientConnectionStateEvent, EAetherClientConnectionState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherClientScreenEvent, EAetherClientScreen, Screen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherClientRequestEvent, const FAetherClientRequestState&, Request);

UCLASS()
class AGEOFAETHER_API UAetherClientSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Client")
    void ResetClient();

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Client")
    void SetConnectionState(EAetherClientConnectionState NewState);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Client")
    void SetScreen(EAetherClientScreen NewScreen);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Client")
    EAetherClientConnectionState GetConnectionState() const { return Snapshot.ConnectionState; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Client")
    EAetherClientScreen GetScreen() const { return Snapshot.Screen; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Client")
    FAetherClientRuntimeSnapshot GetSnapshot() const { return Snapshot; }

    uint32 BeginRequest(const FString& Operation, double NowSeconds, double TimeoutSeconds = 15.0);

    bool CompleteRequest(uint32 RequestID, bool bAccepted);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Client")
    int32 ExpireRequests(double NowSeconds);

    bool GetRequest(uint32 RequestID, FAetherClientRequestState& OutRequest) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Client")
    bool ApplyIdentity(const FAetherClientIdentitySnapshot& Identity, int64 ServerRevision, double ServerTimeSeconds);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Client")
    void ClearIdentity();

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|Client")
    FAetherClientConnectionStateEvent OnConnectionStateChanged;

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|Client")
    FAetherClientScreenEvent OnScreenChanged;

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|Client")
    FAetherClientRequestEvent OnRequestChanged;

private:
    FAetherClientRuntimeSnapshot Snapshot;
    TMap<uint32, FAetherClientRequestState> Requests;
    uint32 NextRequestID = 1;
};
