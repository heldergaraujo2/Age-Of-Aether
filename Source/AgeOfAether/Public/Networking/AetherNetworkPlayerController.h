#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Networking/AetherNetworkTypes.h"
#include "Accounts/AetherAccountSessionTypes.h"
#include "Characters/AetherCharacterTypes.h"
#include "Items/AetherItemTypes.h"

#include "AetherNetworkPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherNetworkResponseEvent, const FAetherNetworkResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherAuthenticationResponseEvent, const FAetherAuthenticationResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherSessionHeartbeatEvent, bool, bAccepted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAetherCharacterOperationEvent, EAetherCharacterOperationResult, Result, const FAetherCharacterRecord&, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherCharacterListEvent, const TArray<FAetherCharacterRecord>&, Characters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherInventoryEvent, const TArray<FAetherInventorySlot>&, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherInventoryOperationEvent, EAetherInventoryOperationResult, Result);

UCLASS()
class AGEOFAETHER_API AAetherNetworkPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AAetherNetworkPlayerController();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Networking")
    void SendNetworkRequest(EAetherNetworkRequestType Type);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Accounts")
    void AuthenticateAccount(const FString& Username, const FString& CredentialProof);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Accounts")
    void ReconnectAccount(const FAetherAccountId& AccountId, const FAetherSessionId& SessionId, const FString& CredentialProof);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Accounts")
    void LogoutAccount();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Accounts")
    void SendSessionHeartbeat();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Character")
    void RequestCharacterList();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Character")
    void CreateCharacter(const FString& Name, EAetherCharacterClass CharacterClass);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Character")
    void SelectCharacter(const FAetherCharacterId& CharacterId);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Character")
    void DeselectCharacter();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Inventory")
    void RequestInventory();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Inventory")
    void MoveInventoryItem(const FAetherItemInstanceId& InstanceId, int32 TargetSlot);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Inventory")
    void SplitInventoryStack(const FAetherItemInstanceId& InstanceId, int32 Quantity, int32 TargetSlot);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Inventory")
    void MergeInventoryStacks(const FAetherItemInstanceId& SourceInstanceId, const FAetherItemInstanceId& TargetInstanceId);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Inventory")
    void DiscardInventoryItem(const FAetherItemInstanceId& InstanceId, int32 Quantity);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Accounts")
    bool IsAccountAuthenticated() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Accounts")
    FAetherAccountId GetAuthenticatedAccountId() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Accounts")
    FAetherSessionId GetSessionId() const;

    UPROPERTY(BlueprintAssignable, Category = "Age of Aether|Networking")
    FAetherNetworkResponseEvent OnNetworkResponse;

    UPROPERTY(BlueprintAssignable, Category = "Age of Aether|Accounts")
    FAetherAuthenticationResponseEvent OnAuthenticationResponse;

    UPROPERTY(BlueprintAssignable, Category = "Age of Aether|Accounts")
    FAetherSessionHeartbeatEvent OnSessionHeartbeat;

    UPROPERTY(BlueprintAssignable, Category = "Age of Aether|Character")
    FAetherCharacterOperationEvent OnCharacterOperation;

    UPROPERTY(BlueprintAssignable, Category = "Age of Aether|Character")
    FAetherCharacterListEvent OnCharacterList;

protected:
    UFUNCTION(Server, Reliable)
    void ServerSubmitRequest(const FAetherNetworkRequest& Request);

    UFUNCTION(Client, Reliable)
    void ClientReceiveResponse(const FAetherNetworkResponse& Response);

    UFUNCTION(Server, Reliable)
    void ServerAuthenticateAccount(uint32 RequestId, const FString& Username, const FString& CredentialProof, const FAetherProtocolVersion& ProtocolVersion);

    UFUNCTION(Client, Reliable)
    void ClientReceiveAuthenticationResponse(uint32 RequestId, const FAetherAuthenticationResponse& Response);

    UFUNCTION(Server, Reliable)
    void ServerReconnectAccount(uint32 RequestId, const FAetherAccountId& AccountId, const FAetherSessionId& SessionId, const FString& CredentialProof, const FAetherProtocolVersion& ProtocolVersion);

    UFUNCTION(Client, Reliable)
    void ClientReceiveReconnectResponse(uint32 RequestId, const FAetherAuthenticationResponse& Response);

    UFUNCTION(Server, Reliable)
    void ServerLogoutAccount(uint32 RequestId, const FAetherSessionId& SessionId, const FAetherProtocolVersion& ProtocolVersion);

    UFUNCTION(Client, Reliable)
    void ClientReceiveLogoutResponse(uint32 RequestId, const FAetherAuthenticationResponse& Response);

    UFUNCTION(Server, Unreliable)
    void ServerSessionHeartbeat(uint32 RequestId, const FAetherSessionId& SessionId, const FAetherProtocolVersion& ProtocolVersion);

    UFUNCTION(Client, Reliable)
    void ClientReceiveSessionHeartbeat(uint32 RequestId, const FAetherAuthenticationResponse& Response);

    UFUNCTION(Server, Reliable)
    void ServerRequestCharacterList(uint32 RequestId);

    UFUNCTION(Client, Reliable)
    void ClientReceiveCharacterList(uint32 RequestId, const TArray<FAetherCharacterRecord>& Characters);

    UFUNCTION(Server, Reliable)
    void ServerCreateCharacter(uint32 RequestId, const FString& Name, EAetherCharacterClass CharacterClass);

    UFUNCTION(Client, Reliable)
    void ClientReceiveCharacterOperation(uint32 RequestId, EAetherCharacterOperationResult Result, const FAetherCharacterRecord& Character);

    UFUNCTION(Server, Reliable)
    void ServerSelectCharacter(uint32 RequestId, const FAetherCharacterId& CharacterId);

    UFUNCTION(Server, Reliable)
    void ServerDeselectCharacter(uint32 RequestId, const FAetherCharacterId& CharacterId);

    virtual void BeginPlay() override;

private:
    bool ValidateRequest(const FAetherNetworkRequest& Request) const;
    bool ValidateAccountRequestId(uint32 RequestId) const;
    double GetServerTimeSeconds() const;
    void ApplyAuthenticatedSession(const FAetherAuthenticationResponse& Response);
    void ClearAuthenticatedSession();

    uint32 NextRequestId = 1;
    uint32 LastProcessedRequestId = 0;
    uint32 NextAccountRequestId = 1;
    uint32 LastProcessedAccountRequestId = 0;
    uint32 NextCharacterRequestId = 1;
    uint32 LastProcessedCharacterRequestId = 0;

    FAetherAccountId AuthenticatedAccountId;
    FAetherSessionId SessionId;
    bool bAccountAuthenticated = false;
};
