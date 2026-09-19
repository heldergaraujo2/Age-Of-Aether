#include "Networking/AetherNetworkPlayerController.h"

#include "Accounts/AetherAccountSessionSubsystem.h"
#include "Characters/AetherCharacterPlayerState.h"
#include "Characters/AetherCharacterSubsystem.h"
#include "Items/AetherItemSubsystem.h"
#include "Progression/AetherProgressionSubsystem.h"
#include "Combat/AetherCombatSubsystem.h"
#include "World/AetherWorldSubsystem.h"
#include "Quests/AetherQuestSubsystem.h"
#include "Social/AetherSocialSubsystem.h"
#include "Economy/AetherEconomySubsystem.h"
#include "Networking/AetherNetworkGameMode.h"
#include "Engine/GameInstance.h"
#include "HAL/PlatformTime.h"
#include "Networking/AetherNetworkGameState.h"
#include "Multiplayer/AetherMultiplayerSubsystem.h"
#include "Security/AetherSecuritySubsystem.h"


namespace
{
    bool GetControllerCharacter(AAetherNetworkPlayerController* Controller, FAetherCharacterRecord& OutCharacter)
    {
        if (!Controller || !Controller->IsAccountAuthenticated())
        {
            return false;
        }

        const AAetherCharacterPlayerState* State = Controller->GetPlayerState<AAetherCharacterPlayerState>();
        if (!State || !State->GetCharacterId().IsValid())
        {
            return false;
        }

        UAetherCharacterSubsystem* Characters = Controller->GetGameInstance()
            ? Controller->GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
            : nullptr;
        return Characters && Characters->FindCharacter(State->GetCharacterId(), OutCharacter)
            && OutCharacter.AccountId == Controller->GetAuthenticatedAccountId();
    }
}

AAetherNetworkPlayerController::AAetherNetworkPlayerController()
{
    bReplicates = true;
}

void AAetherNetworkPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AAetherNetworkPlayerController::SendNetworkRequest(EAetherNetworkRequestType Type)
{
    FAetherNetworkRequest Request;
    Request.RequestId = NextRequestId++;
    Request.Type = Type;
    Request.ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerSubmitRequest_Implementation(Request);
        return;
    }

    ServerSubmitRequest(Request);
}

void AAetherNetworkPlayerController::AuthenticateAccount(const FString& Username, const FString& CredentialProof)
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerAuthenticateAccount_Implementation(RequestId, Username, CredentialProof, ProtocolVersion);
        return;
    }

    ServerAuthenticateAccount(RequestId, Username, CredentialProof, ProtocolVersion);
}

void AAetherNetworkPlayerController::ReconnectAccount(
    const FAetherAccountId& AccountId,
    const FAetherSessionId& InSessionId,
    const FString& CredentialProof)
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerReconnectAccount_Implementation(RequestId, AccountId, InSessionId, CredentialProof, ProtocolVersion);
        return;
    }

    ServerReconnectAccount(RequestId, AccountId, InSessionId, CredentialProof, ProtocolVersion);
}

void AAetherNetworkPlayerController::LogoutAccount()
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        ServerLogoutAccount_Implementation(RequestId, SessionId, ProtocolVersion);
        return;
    }

    ServerLogoutAccount(RequestId, SessionId, ProtocolVersion);
}

void AAetherNetworkPlayerController::SendSessionHeartbeat()
{
    const uint32 RequestId = NextAccountRequestId++;
    const FAetherProtocolVersion ProtocolVersion = FAetherProtocolVersion::Current();

    if (HasAuthority())
    {
        UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
            : nullptr;

        const bool bAccepted = Sessions && bAccountAuthenticated
            && Sessions->Heartbeat(SessionId, ProtocolVersion, GetServerTimeSeconds());

        FAetherAuthenticationResponse Response;
        Response.Result = bAccepted
            ? EAetherAuthenticationResult::Accepted
            : EAetherAuthenticationResult::SessionNotFound;
        Response.AccountId = AuthenticatedAccountId;
        Response.SessionId = SessionId;
        ClientReceiveSessionHeartbeat(RequestId, Response);
        return;
    }

    ServerSessionHeartbeat(RequestId, SessionId, ProtocolVersion);
}

bool AAetherNetworkPlayerController::IsAccountAuthenticated() const
{
    return bAccountAuthenticated;
}

FAetherAccountId AAetherNetworkPlayerController::GetAuthenticatedAccountId() const
{
    return AuthenticatedAccountId;
}

FAetherSessionId AAetherNetworkPlayerController::GetSessionId() const
{
    return SessionId;
}

void AAetherNetworkPlayerController::ServerSubmitRequest_Implementation(const FAetherNetworkRequest& Request)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Generic))
    {
        return;
    }
    FAetherNetworkResponse Response;
    Response.RequestId = Request.RequestId;
    Response.ProtocolVersion = FAetherProtocolVersion::Current();

    if (!ValidateRequest(Request))
    {
        Response.Result = Request.ProtocolVersion.IsCompatibleWith(Response.ProtocolVersion)
            ? EAetherNetworkResultCode::InvalidRequest
            : EAetherNetworkResultCode::InvalidProtocol;

        ClientReceiveResponse(Response);
        return;
    }

    const AAetherNetworkGameState* NetworkState = GetWorld()
        ? GetWorld()->GetGameState<AAetherNetworkGameState>()
        : nullptr;

    if (!NetworkState)
    {
        Response.Result = EAetherNetworkResultCode::ServerUnavailable;
        ClientReceiveResponse(Response);
        return;
    }

    LastProcessedRequestId = Request.RequestId;
    Response.Result = EAetherNetworkResultCode::Accepted;
    Response.AuthoritativeStateRevision = NetworkState->GetAuthoritativeStateRevision();

    if (Request.Type == EAetherNetworkRequestType::Heartbeat)
    {
        if (AAetherNetworkGameState* MutableState = GetWorld()->GetGameState<AAetherNetworkGameState>())
        {
            MutableState->AdvanceAuthoritativeState();
            Response.AuthoritativeStateRevision = MutableState->GetAuthoritativeStateRevision();
        }
    }

    ClientReceiveResponse(Response);
}

void AAetherNetworkPlayerController::ClientReceiveResponse_Implementation(const FAetherNetworkResponse& Response)
{
    OnNetworkResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerAuthenticateAccount_Implementation(
    uint32 RequestId,
    const FString& Username,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Authentication))
    {
        return;
    }
    if (!ValidateAccountRequestId(RequestId))
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        ClientReceiveAuthenticationResponse(RequestId, Response);
        return;
    }

    if (bAccountAuthenticated)
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::AlreadyOnline;
        Response.AccountId = AuthenticatedAccountId;
        Response.SessionId = SessionId;
        ClientReceiveAuthenticationResponse(RequestId, Response);
        return;
    }

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    FAetherAuthenticationResponse Response;
    if (!Sessions)
    {
        Response.Result = EAetherAuthenticationResult::AuthenticationUnavailable;
    }
    else
    {
        Response = Sessions->Authenticate(
            Username,
            CredentialProof,
            ProtocolVersion,
            GetServerTimeSeconds());

        if (Response.Result == EAetherAuthenticationResult::Accepted)
        {
            AuthenticatedAccountId = Response.AccountId;
            SessionId = Response.SessionId;
            bAccountAuthenticated = true;
            if (UAetherMultiplayerSubsystem* Multiplayer = GetGameInstance()->GetSubsystem<UAetherMultiplayerSubsystem>())
                Multiplayer->SetAuthenticated(GetUniqueID(), true);
        }
    }

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveAuthenticationResponse(RequestId, Response);
}

void AAetherNetworkPlayerController::ClientReceiveAuthenticationResponse_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    if (Response.Result == EAetherAuthenticationResult::Accepted)
    {
        ApplyAuthenticatedSession(Response);
    }

    OnAuthenticationResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerReconnectAccount_Implementation(
    uint32 RequestId,
    const FAetherAccountId& AccountId,
    const FAetherSessionId& InSessionId,
    const FString& CredentialProof,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Authentication))
    {
        return;
    }
    if (!ValidateAccountRequestId(RequestId))
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        ClientReceiveReconnectResponse(RequestId, Response);
        return;
    }

    if (bAccountAuthenticated)
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::AlreadyOnline;
        Response.AccountId = AuthenticatedAccountId;
        Response.SessionId = SessionId;
        ClientReceiveReconnectResponse(RequestId, Response);
        return;
    }

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    FAetherAuthenticationResponse Response;
    if (!Sessions)
    {
        Response.Result = EAetherAuthenticationResult::AuthenticationUnavailable;
    }
    else
    {
        Response = Sessions->Reconnect(
            AccountId,
            InSessionId,
            CredentialProof,
            ProtocolVersion,
            GetServerTimeSeconds());

        if (Response.Result == EAetherAuthenticationResult::Accepted)
        {
            AuthenticatedAccountId = Response.AccountId;
            SessionId = Response.SessionId;
            bAccountAuthenticated = true;
            if (UAetherMultiplayerSubsystem* Multiplayer = GetGameInstance()->GetSubsystem<UAetherMultiplayerSubsystem>())
                Multiplayer->SetAuthenticated(GetUniqueID(), true);
        }
    }

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveReconnectResponse(RequestId, Response);
}

void AAetherNetworkPlayerController::ClientReceiveReconnectResponse_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    if (Response.Result == EAetherAuthenticationResult::Accepted)
    {
        ApplyAuthenticatedSession(Response);
    }

    OnAuthenticationResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerLogoutAccount_Implementation(
    uint32 RequestId,
    const FAetherSessionId& InSessionId,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Session))
    {
        return;
    }
    if (!ValidateAccountRequestId(RequestId))
    {
        FAetherAuthenticationResponse Response;
        Response.Result = EAetherAuthenticationResult::InvalidRequest;
        ClientReceiveLogoutResponse(RequestId, Response);
        return;
    }

    FAetherAuthenticationResponse Response;
    Response.AccountId = AuthenticatedAccountId;
    Response.SessionId = InSessionId;

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;

    const bool bLoggedOut = Sessions
        && bAccountAuthenticated
        && InSessionId == SessionId
        && Sessions->Logout(InSessionId, ProtocolVersion, GetServerTimeSeconds());

    Response.Result = bLoggedOut
        ? EAetherAuthenticationResult::Accepted
        : EAetherAuthenticationResult::SessionNotFound;

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveLogoutResponse(RequestId, Response);

    if (bLoggedOut)
    {
        if (UAetherMultiplayerSubsystem* Multiplayer = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherMultiplayerSubsystem>() : nullptr)
        {
            Multiplayer->SetAuthenticated(GetUniqueID(), false);
            Multiplayer->Heartbeat(GetUniqueID(), GetServerTimeSeconds());
        }

        if (UAetherCharacterSubsystem* Characters = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
            : nullptr)
        {
            if (const AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>())
            {
                Characters->DeselectCharacter(
                    AuthenticatedAccountId,
                    State->GetCharacterId());
            }
        }

        if (APawn* CurrentPawn = GetPawn())
        {
            CurrentPawn->Destroy();
        }

        if (UAetherSocialSubsystem* Social = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>()
            : nullptr)
        {
            if (const AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>())
            {
                Social->ClearCharacterState(State->GetCharacterId());
            }
            Social->ClearAccountState(AuthenticatedAccountId);
        }

        ClearAuthenticatedSession();
    }
}

void AAetherNetworkPlayerController::ClientReceiveLogoutResponse_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    if (Response.Result == EAetherAuthenticationResult::Accepted)
    {
        ClearAuthenticatedSession();
    }

    OnAuthenticationResponse.Broadcast(Response);
}

void AAetherNetworkPlayerController::ServerSessionHeartbeat_Implementation(
    uint32 RequestId,
    const FAetherSessionId& InSessionId,
    const FAetherProtocolVersion& ProtocolVersion)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Session))
    {
        return;
    }
    if (!ValidateAccountRequestId(RequestId))
    {
        return;
    }

    UAetherAccountSessionSubsystem* Sessions = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherAccountSessionSubsystem>()
        : nullptr;
    UAetherMultiplayerSubsystem* Multiplayer = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherMultiplayerSubsystem>()
        : nullptr;

    const double Now = GetServerTimeSeconds();
    const bool bAuthorityHeartbeat = Multiplayer && Multiplayer->Heartbeat(GetUniqueID(), Now);

    if (UAetherSecuritySubsystem* Security = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherSecuritySubsystem>() : nullptr)
    {
        if (const APawn* Pawn = GetPawn())
        {
            Security->ValidateMovement(GetUniqueID(), Pawn->GetActorLocation(), Now);
        }
    }

    FAetherAuthenticationResponse Response;
    Response.AccountId = AuthenticatedAccountId;
    Response.SessionId = InSessionId;
    Response.Result = Sessions
        && bAccountAuthenticated
        && InSessionId == SessionId
        && bAuthorityHeartbeat
        && Sessions->Heartbeat(InSessionId, ProtocolVersion, Now)
        ? EAetherAuthenticationResult::Accepted
        : EAetherAuthenticationResult::SessionNotFound;

    LastProcessedAccountRequestId = RequestId;
    ClientReceiveSessionHeartbeat(RequestId, Response);
}

void AAetherNetworkPlayerController::ClientReceiveSessionHeartbeat_Implementation(
    uint32 RequestId,
    const FAetherAuthenticationResponse& Response)
{
    OnSessionHeartbeat.Broadcast(Response.Result == EAetherAuthenticationResult::Accepted);
}

void AAetherNetworkPlayerController::RequestCharacterList()
{
    const uint32 RequestId = NextCharacterRequestId++;

    if (HasAuthority())
    {
        ServerRequestCharacterList_Implementation(RequestId);
        return;
    }

    ServerRequestCharacterList(RequestId);
}

void AAetherNetworkPlayerController::CreateCharacter(
    const FString& Name,
    EAetherCharacterClass CharacterClass)
{
    const uint32 RequestId = NextCharacterRequestId++;

    if (HasAuthority())
    {
        ServerCreateCharacter_Implementation(RequestId, Name, CharacterClass);
        return;
    }

    ServerCreateCharacter(RequestId, Name, CharacterClass);
}

void AAetherNetworkPlayerController::SelectCharacter(const FAetherCharacterId& CharacterId)
{
    const uint32 RequestId = NextCharacterRequestId++;

    if (HasAuthority())
    {
        ServerSelectCharacter_Implementation(RequestId, CharacterId);
        return;
    }

    ServerSelectCharacter(RequestId, CharacterId);
}

void AAetherNetworkPlayerController::DeselectCharacter()
{
    const uint32 RequestId = NextCharacterRequestId++;

    FAetherCharacterId CharacterId;
    if (const AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>())
    {
        CharacterId = State->GetCharacterId();
    }

    if (HasAuthority())
    {
        ServerDeselectCharacter_Implementation(RequestId, CharacterId);
        return;
    }

    ServerDeselectCharacter(RequestId, CharacterId);
}

void AAetherNetworkPlayerController::ServerRequestCharacterList_Implementation(uint32 RequestId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Character))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedCharacterRequestId)
    {
        return;
    }

    if (!bAccountAuthenticated)
    {
        ClientReceiveCharacterList(RequestId, TArray<FAetherCharacterRecord>());
        return;
    }

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    TArray<FAetherCharacterRecord> CharacterList;
    if (Characters)
    {
        Characters->GetCharactersForAccount(AuthenticatedAccountId, CharacterList);
    }

    LastProcessedCharacterRequestId = RequestId;
    ClientReceiveCharacterList(RequestId, CharacterList);
}

void AAetherNetworkPlayerController::ClientReceiveCharacterList_Implementation(
    uint32 RequestId,
    const TArray<FAetherCharacterRecord>& Characters)
{
    OnCharacterList.Broadcast(Characters);
}

void AAetherNetworkPlayerController::ServerCreateCharacter_Implementation(
    uint32 RequestId,
    const FString& Name,
    EAetherCharacterClass CharacterClass)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Character))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedCharacterRequestId)
    {
        return;
    }

    FAetherCharacterRecord Character;
    EAetherCharacterOperationResult Result = EAetherCharacterOperationResult::NotAuthenticated;

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (bAccountAuthenticated && Characters)
    {
        if (Characters->CreateCharacter(AuthenticatedAccountId, Name, CharacterClass, Character))
        {
            Result = EAetherCharacterOperationResult::Accepted;
        }
        else
        {
            FAetherCharacterRecord Existing;
            if (Characters->FindCharacterByName(Name, Existing))
            {
                Result = Existing.AccountId == AuthenticatedAccountId
                    ? EAetherCharacterOperationResult::NameUnavailable
                    : EAetherCharacterOperationResult::NameUnavailable;
            }
            else if (Name.TrimStartAndEnd().Len() < 3 || Name.TrimStartAndEnd().Len() > 16)
            {
                Result = EAetherCharacterOperationResult::InvalidName;
            }
            else if (Characters->NumCharactersForAccount(AuthenticatedAccountId) >= FAetherCharacterService::MaxCharactersPerAccount)
            {
                Result = EAetherCharacterOperationResult::CharacterLimitReached;
            }
            else
            {
                Result = EAetherCharacterOperationResult::InvalidRequest;
            }
        }
    }

    LastProcessedCharacterRequestId = RequestId;
    ClientReceiveCharacterOperation(RequestId, Result, Character);
}

void AAetherNetworkPlayerController::ServerSelectCharacter_Implementation(
    uint32 RequestId,
    const FAetherCharacterId& CharacterId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Character))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedCharacterRequestId)
    {
        return;
    }

    FAetherCharacterRecord Character;
    EAetherCharacterOperationResult Result = EAetherCharacterOperationResult::NotAuthenticated;

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (bAccountAuthenticated && Characters)
    {
        if (!Characters->FindCharacter(CharacterId, Character))
        {
            Result = EAetherCharacterOperationResult::CharacterNotFound;
        }
        else if (Character.AccountId != AuthenticatedAccountId)
        {
            Result = EAetherCharacterOperationResult::NotOwned;
        }
        else if (Character.Status == EAetherCharacterStatus::Disabled)
        {
            Result = EAetherCharacterOperationResult::CharacterDisabled;
        }
        else if (Character.Status == EAetherCharacterStatus::Deleted)
        {
            Result = EAetherCharacterOperationResult::CharacterDeleted;
        }
        else if (Characters->SelectCharacter(AuthenticatedAccountId, CharacterId, Character))
        {
            Result = EAetherCharacterOperationResult::Accepted;

            if (AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>())
            {
                State->SetCharacterIdentity(Character);
            }

            if (AAetherNetworkGameMode* GameMode = GetWorld()->GetAuthGameMode<AAetherNetworkGameMode>())
            {
                GameMode->SpawnSelectedCharacter(this, Character);
            }
        }
        else
        {
            Result = EAetherCharacterOperationResult::AnotherCharacterSelected;
        }
    }

    LastProcessedCharacterRequestId = RequestId;
    ClientReceiveCharacterOperation(RequestId, Result, Character);
}

void AAetherNetworkPlayerController::ServerDeselectCharacter_Implementation(
    uint32 RequestId,
    const FAetherCharacterId& CharacterId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Character))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedCharacterRequestId)
    {
        return;
    }

    FAetherCharacterRecord Character;
    EAetherCharacterOperationResult Result = EAetherCharacterOperationResult::NotAuthenticated;

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (bAccountAuthenticated && Characters)
    {
        if (Characters->DeselectCharacter(AuthenticatedAccountId, CharacterId))
        {
            Result = EAetherCharacterOperationResult::Accepted;

            if (APawn* CurrentPawn = GetPawn())
            {
                CurrentPawn->Destroy();
            }
        }
        else
        {
            Result = Characters->FindCharacter(CharacterId, Character)
                ? EAetherCharacterOperationResult::NotOwned
                : EAetherCharacterOperationResult::CharacterNotFound;

            if (Result == EAetherCharacterOperationResult::NotOwned &&
                Character.AccountId == AuthenticatedAccountId)
            {
                Result = EAetherCharacterOperationResult::InvalidRequest;
            }
        }
    }

    LastProcessedCharacterRequestId = RequestId;
    ClientReceiveCharacterOperation(RequestId, Result, Character);
}

void AAetherNetworkPlayerController::ClientReceiveCharacterOperation_Implementation(
    uint32 RequestId,
    EAetherCharacterOperationResult Result,
    const FAetherCharacterRecord& Character)
{
    OnCharacterOperation.Broadcast(Result, Character);
}


void AAetherNetworkPlayerController::RequestInventory()
{
    const uint32 RequestId = NextInventoryRequestId++;
    if (HasAuthority())
    {
        ServerRequestInventory_Implementation(RequestId);
        return;
    }
    ServerRequestInventory(RequestId);
}

void AAetherNetworkPlayerController::MoveInventoryItem(const FAetherItemInstanceId& InstanceId, int32 TargetSlot)
{
    const uint32 RequestId = NextInventoryRequestId++;
    if (HasAuthority())
    {
        ServerMoveInventoryItem_Implementation(RequestId, InstanceId, TargetSlot);
        return;
    }
    ServerMoveInventoryItem(RequestId, InstanceId, TargetSlot);
}

void AAetherNetworkPlayerController::SplitInventoryStack(const FAetherItemInstanceId& InstanceId, int32 Quantity, int32 TargetSlot)
{
    const uint32 RequestId = NextInventoryRequestId++;
    if (HasAuthority())
    {
        ServerSplitInventoryStack_Implementation(RequestId, InstanceId, Quantity, TargetSlot);
        return;
    }
    ServerSplitInventoryStack(RequestId, InstanceId, Quantity, TargetSlot);
}

void AAetherNetworkPlayerController::MergeInventoryStacks(const FAetherItemInstanceId& SourceInstanceId, const FAetherItemInstanceId& TargetInstanceId)
{
    const uint32 RequestId = NextInventoryRequestId++;
    if (HasAuthority())
    {
        ServerMergeInventoryStacks_Implementation(RequestId, SourceInstanceId, TargetInstanceId);
        return;
    }
    ServerMergeInventoryStacks(RequestId, SourceInstanceId, TargetInstanceId);
}

void AAetherNetworkPlayerController::DiscardInventoryItem(const FAetherItemInstanceId& InstanceId, int32 Quantity)
{
    const uint32 RequestId = NextInventoryRequestId++;
    if (HasAuthority())
    {
        ServerDiscardInventoryItem_Implementation(RequestId, InstanceId, Quantity);
        return;
    }
    ServerDiscardInventoryItem(RequestId, InstanceId, Quantity);
}

namespace
{
    bool ResolveAuthenticatedCharacter(
        AAetherNetworkPlayerController* Controller,
        FAetherCharacterId& OutCharacterId)
    {
        OutCharacterId = FAetherCharacterId();
        if (!Controller || !Controller->IsAccountAuthenticated())
        {
            return false;
        }

        const AAetherCharacterPlayerState* State = Controller->GetPlayerState<AAetherCharacterPlayerState>();
        if (!State || !State->GetCharacterId().IsValid())
        {
            return false;
        }

        OutCharacterId = State->GetCharacterId();
        return true;
    }
}

void AAetherNetworkPlayerController::ServerRequestInventory_Implementation(uint32 RequestId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Inventory))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedInventoryRequestId) return;

    TArray<FAetherInventorySlot> Inventory;
    EAetherInventoryOperationResult Result = EAetherInventoryOperationResult::NotAuthenticated;
    FAetherCharacterId CharacterId;

    UAetherItemSubsystem* Items = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>()
        : nullptr;

    if (ResolveAuthenticatedCharacter(this, CharacterId) && Items)
    {
        Result = Items->GetInventory(CharacterId, Inventory)
            ? EAetherInventoryOperationResult::Accepted
            : EAetherInventoryOperationResult::InvalidRequest;
    }

    LastProcessedInventoryRequestId = RequestId;
    ClientReceiveInventory(RequestId, Inventory, Result);
}

void AAetherNetworkPlayerController::ServerMoveInventoryItem_Implementation(
    uint32 RequestId,
    const FAetherItemInstanceId& InstanceId,
    int32 TargetSlot)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Inventory))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedInventoryRequestId) return;

    TArray<FAetherInventorySlot> Inventory;
    EAetherInventoryOperationResult Result = EAetherInventoryOperationResult::NotAuthenticated;
    FAetherCharacterId CharacterId;

    UAetherItemSubsystem* Items = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>()
        : nullptr;

    if (ResolveAuthenticatedCharacter(this, CharacterId) && Items)
    {
        Result = Items->MoveItem(CharacterId, InstanceId, TargetSlot, Inventory)
            ? EAetherInventoryOperationResult::Accepted
            : EAetherInventoryOperationResult::InvalidRequest;
    }

    LastProcessedInventoryRequestId = RequestId;
    ClientReceiveInventory(RequestId, Inventory, Result);
}

void AAetherNetworkPlayerController::ServerSplitInventoryStack_Implementation(
    uint32 RequestId,
    const FAetherItemInstanceId& InstanceId,
    int32 Quantity,
    int32 TargetSlot)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Inventory))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedInventoryRequestId) return;

    TArray<FAetherInventorySlot> Inventory;
    EAetherInventoryOperationResult Result = EAetherInventoryOperationResult::NotAuthenticated;
    FAetherCharacterId CharacterId;

    UAetherItemSubsystem* Items = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>()
        : nullptr;

    if (ResolveAuthenticatedCharacter(this, CharacterId) && Items)
    {
        Result = Items->SplitStack(CharacterId, InstanceId, Quantity, TargetSlot, Inventory)
            ? EAetherInventoryOperationResult::Accepted
            : EAetherInventoryOperationResult::InvalidRequest;
    }

    LastProcessedInventoryRequestId = RequestId;
    ClientReceiveInventory(RequestId, Inventory, Result);
}

void AAetherNetworkPlayerController::ServerMergeInventoryStacks_Implementation(
    uint32 RequestId,
    const FAetherItemInstanceId& SourceInstanceId,
    const FAetherItemInstanceId& TargetInstanceId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Inventory))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedInventoryRequestId) return;

    TArray<FAetherInventorySlot> Inventory;
    EAetherInventoryOperationResult Result = EAetherInventoryOperationResult::NotAuthenticated;
    FAetherCharacterId CharacterId;

    UAetherItemSubsystem* Items = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>()
        : nullptr;

    if (ResolveAuthenticatedCharacter(this, CharacterId) && Items)
    {
        Result = Items->MergeStacks(CharacterId, SourceInstanceId, TargetInstanceId, Inventory)
            ? EAetherInventoryOperationResult::Accepted
            : EAetherInventoryOperationResult::InvalidRequest;
    }

    LastProcessedInventoryRequestId = RequestId;
    ClientReceiveInventory(RequestId, Inventory, Result);
}

void AAetherNetworkPlayerController::ServerDiscardInventoryItem_Implementation(
    uint32 RequestId,
    const FAetherItemInstanceId& InstanceId,
    int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Inventory))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedInventoryRequestId) return;

    TArray<FAetherInventorySlot> Inventory;
    EAetherInventoryOperationResult Result = EAetherInventoryOperationResult::NotAuthenticated;
    FAetherCharacterId CharacterId;

    UAetherItemSubsystem* Items = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>()
        : nullptr;

    if (ResolveAuthenticatedCharacter(this, CharacterId) && Items)
    {
        Result = Items->RemoveItem(CharacterId, InstanceId, Quantity, Inventory)
            ? EAetherInventoryOperationResult::Accepted
            : EAetherInventoryOperationResult::InvalidRequest;
    }

    LastProcessedInventoryRequestId = RequestId;
    ClientReceiveInventory(RequestId, Inventory, Result);
}

void AAetherNetworkPlayerController::ClientReceiveInventory_Implementation(
    uint32 RequestId,
    const TArray<FAetherInventorySlot>& Inventory,
    EAetherInventoryOperationResult Result)
{
    OnInventoryOperation.Broadcast(Result);
    if (Result == EAetherInventoryOperationResult::Accepted)
    {
        OnInventory.Broadcast(Inventory);
    }
}


void AAetherNetworkPlayerController::BasicAttack(const FAetherCharacterId& TargetCharacterId)
{
    const uint32 RequestId = NextCombatRequestId++;
    if (HasAuthority())
    {
        ServerBasicAttack_Implementation(RequestId, TargetCharacterId);
        return;
    }

    ServerBasicAttack(RequestId, TargetCharacterId);
}

void AAetherNetworkPlayerController::ServerBasicAttack_Implementation(
    uint32 RequestId,
    const FAetherCharacterId& TargetCharacterId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Combat))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedCombatRequestId)
    {
        return;
    }

    FAetherCombatResult Result;
    Result.RequestId = RequestId;

    UAetherCombatSubsystem* Combat = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCombatSubsystem>()
        : nullptr;

    const AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>();
    const FAetherCharacterId AttackerId = State ? State->GetCharacterId() : FAetherCharacterId();

    bool bResolved = false;
    if (Combat && bAccountAuthenticated && AttackerId.IsValid() && TargetCharacterId.IsValid())
    {
        bResolved = Combat->BasicAttack(
            AuthenticatedAccountId,
            AttackerId,
            TargetCharacterId,
            RequestId,
            GetServerTimeSeconds(),
            Result);
    }
    else
    {
        Result.Result = EAetherCombatResultCode::NotAuthenticated;
    }

    LastProcessedCombatRequestId = RequestId;

    if (bResolved && Result.Result != EAetherCombatResultCode::Missed)
    {
        UAetherCharacterSubsystem* Characters = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
            : nullptr;

        if (Characters)
        {
            FAetherCharacterRecord TargetCharacter;
            if (Characters->FindCharacter(TargetCharacterId, TargetCharacter))
            {
                if (AAetherNetworkGameState* NetworkState = GetWorld()
                    ? GetWorld()->GetGameState<AAetherNetworkGameState>()
                    : nullptr)
                {
                    for (APlayerState* PlayerStateBase : NetworkState->PlayerArray)
                    {
                        if (AAetherCharacterPlayerState* TargetState = Cast<AAetherCharacterPlayerState>(PlayerStateBase))
                        {
                            if (TargetState->GetCharacterId() == TargetCharacterId)
                            {
                                TargetState->SetCharacterIdentity(TargetCharacter);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    ClientReceiveCombat(RequestId, Result);
}

void AAetherNetworkPlayerController::ClientReceiveCombat_Implementation(
    uint32 RequestId,
    const FAetherCombatResult& Result)
{
    OnCombat.Broadcast(Result);
}

void AAetherNetworkPlayerController::RequestWorldTransition(const FAetherWorldZoneId& TargetZoneId)
{
    const uint32 RequestId = NextWorldRequestId++;
    if (HasAuthority())
    {
        ServerRequestWorldTransition_Implementation(RequestId, TargetZoneId);
        return;
    }

    ServerRequestWorldTransition(RequestId, TargetZoneId);
}

void AAetherNetworkPlayerController::ServerRequestWorldTransition_Implementation(
    uint32 RequestId,
    const FAetherWorldZoneId& TargetZoneId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::World))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedWorldRequestId)
    {
        return;
    }

    FAetherWorldTransitionResult Result;
    Result.Result = EAetherWorldTransitionResult::NotAuthenticated;

    const AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>();
    const FAetherCharacterId CharacterId = State
        ? State->GetCharacterId()
        : FAetherCharacterId();

    UAetherWorldSubsystem* World = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherWorldSubsystem>()
        : nullptr;

    bool bAccepted = false;
    if (World && bAccountAuthenticated && CharacterId.IsValid() && TargetZoneId.IsValid())
    {
        bAccepted = World->TransitionCharacter(
            AuthenticatedAccountId,
            CharacterId,
            TargetZoneId,
            Result);
    }
    else if (!bAccountAuthenticated)
    {
        Result.Result = EAetherWorldTransitionResult::NotAuthenticated;
    }
    else if (!CharacterId.IsValid())
    {
        Result.Result = EAetherWorldTransitionResult::CharacterNotSelected;
    }
    else
    {
        Result.Result = EAetherWorldTransitionResult::InvalidRequest;
    }

    LastProcessedWorldRequestId = RequestId;

    if (bAccepted)
    {
        if (AAetherNetworkGameState* NetworkState = GetWorld()
            ? GetWorld()->GetGameState<AAetherNetworkGameState>()
            : nullptr)
        {
            NetworkState->AdvanceAuthoritativeState();
        }

        UAetherCharacterSubsystem* Characters = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
            : nullptr;

        FAetherCharacterRecord Character;
        if (Characters && Characters->FindCharacter(CharacterId, Character))
        {
            if (AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>())
            {
                CharacterState->SetCharacterIdentity(Character);
            }

            if (APawn* Pawn = GetPawn())
            {
                Pawn->SetActorLocationAndRotation(
                    Character.WorldLocation,
                    Character.WorldRotation,
                    false,
                    nullptr,
                    ETeleportType::TeleportPhysics);
            }
        }
    }

    ClientReceiveWorldTransition(RequestId, Result);
}

void AAetherNetworkPlayerController::ClientReceiveWorldTransition_Implementation(
    uint32 RequestId,
    const FAetherWorldTransitionResult& Result)
{
    OnWorldTransition.Broadcast(Result);
}


void AAetherNetworkPlayerController::RequestQuestList()
{
    const uint32 RequestId = NextQuestRequestId++;
    if (HasAuthority())
    {
        ServerRequestQuestList_Implementation(RequestId);
        return;
    }
    ServerRequestQuestList(RequestId);
}

void AAetherNetworkPlayerController::AcceptQuest(const FAetherQuestId& QuestId)
{
    const uint32 RequestId = NextQuestRequestId++;
    if (HasAuthority())
    {
        ServerAcceptQuest_Implementation(RequestId, QuestId);
        return;
    }
    ServerAcceptQuest(RequestId, QuestId);
}

void AAetherNetworkPlayerController::AbandonQuest(const FAetherQuestId& QuestId)
{
    const uint32 RequestId = NextQuestRequestId++;
    if (HasAuthority())
    {
        ServerAbandonQuest_Implementation(RequestId, QuestId);
        return;
    }
    ServerAbandonQuest(RequestId, QuestId);
}

void AAetherNetworkPlayerController::CompleteQuest(const FAetherQuestId& QuestId)
{
    const uint32 RequestId = NextQuestRequestId++;
    if (HasAuthority())
    {
        ServerCompleteQuest_Implementation(RequestId, QuestId);
        return;
    }
    ServerCompleteQuest(RequestId, QuestId);
}

void AAetherNetworkPlayerController::ServerRequestQuestList_Implementation(uint32 RequestId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Quest))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedQuestRequestId)
    {
        return;
    }

    TArray<FAetherQuestState> States;
    if (bAccountAuthenticated)
    {
        const AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>();
        const FAetherCharacterId CharacterId = CharacterState ? CharacterState->GetCharacterId() : FAetherCharacterId();

        if (UAetherQuestSubsystem* Quests = GetGameInstance()
            ? GetGameInstance()->GetSubsystem<UAetherQuestSubsystem>()
            : nullptr)
        {
            Quests->GetQuestStates(CharacterId, States);
        }
    }

    LastProcessedQuestRequestId = RequestId;
    ClientReceiveQuestList(RequestId, States);
}

void AAetherNetworkPlayerController::ServerAcceptQuest_Implementation(uint32 RequestId, const FAetherQuestId& QuestId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Quest))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedQuestRequestId)
    {
        return;
    }

    FAetherQuestOperation Operation;
    Operation.Result = EAetherQuestOperationResult::CharacterInvalid;

    const AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>();
    const FAetherCharacterId CharacterId = CharacterState ? CharacterState->GetCharacterId() : FAetherCharacterId();

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;
    UAetherQuestSubsystem* Quests = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherQuestSubsystem>()
        : nullptr;

    FAetherCharacterRecord Character;
    if (bAccountAuthenticated && Characters && Quests && Characters->FindCharacter(CharacterId, Character))
    {
        if (Character.AccountId == AuthenticatedAccountId)
        {
            Quests->AcceptQuest(Character, QuestId, Operation);
        }
        else
        {
            Operation.Result = EAetherQuestOperationResult::CharacterNotOwned;
        }
    }

    LastProcessedQuestRequestId = RequestId;
    ClientReceiveQuestOperation(RequestId, Operation);
}

void AAetherNetworkPlayerController::ServerAbandonQuest_Implementation(uint32 RequestId, const FAetherQuestId& QuestId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Quest))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedQuestRequestId)
    {
        return;
    }

    FAetherQuestOperation Operation;
    Operation.Result = EAetherQuestOperationResult::CharacterInvalid;

    const AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>();
    const FAetherCharacterId CharacterId = CharacterState ? CharacterState->GetCharacterId() : FAetherCharacterId();

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;
    UAetherQuestSubsystem* Quests = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherQuestSubsystem>()
        : nullptr;

    FAetherCharacterRecord Character;
    if (bAccountAuthenticated && Characters && Quests && Characters->FindCharacter(CharacterId, Character))
    {
        if (Character.AccountId == AuthenticatedAccountId)
        {
            Quests->AbandonQuest(Character, QuestId, Operation);
        }
        else
        {
            Operation.Result = EAetherQuestOperationResult::CharacterNotOwned;
        }
    }

    LastProcessedQuestRequestId = RequestId;
    ClientReceiveQuestOperation(RequestId, Operation);
}

void AAetherNetworkPlayerController::ServerCompleteQuest_Implementation(uint32 RequestId, const FAetherQuestId& QuestId)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Quest))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedQuestRequestId)
    {
        return;
    }

    FAetherQuestOperation Operation;
    Operation.Result = EAetherQuestOperationResult::CharacterInvalid;

    const AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>();
    const FAetherCharacterId CharacterId = CharacterState ? CharacterState->GetCharacterId() : FAetherCharacterId();

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;
    UAetherQuestSubsystem* Quests = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherQuestSubsystem>()
        : nullptr;

    FAetherCharacterRecord Character;
    if (bAccountAuthenticated && Characters && Quests && Characters->FindCharacter(CharacterId, Character))
    {
        if (Character.AccountId == AuthenticatedAccountId)
        {
            Quests->CompleteQuest(Character, QuestId, Operation);
        }
        else
        {
            Operation.Result = EAetherQuestOperationResult::CharacterNotOwned;
        }
    }

    LastProcessedQuestRequestId = RequestId;
    ClientReceiveQuestOperation(RequestId, Operation);
}

void AAetherNetworkPlayerController::ClientReceiveQuestList_Implementation(
    uint32 RequestId,
    const TArray<FAetherQuestState>& States)
{
    OnQuestList.Broadcast(States);
}

void AAetherNetworkPlayerController::ClientReceiveQuestOperation_Implementation(
    uint32 RequestId,
    const FAetherQuestOperation& Operation)
{
    OnQuestOperation.Broadcast(Operation);
}


void AAetherNetworkPlayerController::RequestFriends(){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerRequestFriends_Implementation(Id);else ServerRequestFriends(Id);}
void AAetherNetworkPlayerController::SendFriendRequest(const FAetherAccountId& Target){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerSendFriendRequest_Implementation(Id,Target);else ServerSendFriendRequest(Id,Target);}
void AAetherNetworkPlayerController::AcceptFriendRequest(const FAetherAccountId& Sender){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerAcceptFriendRequest_Implementation(Id,Sender);else ServerAcceptFriendRequest(Id,Sender);}
void AAetherNetworkPlayerController::RejectFriendRequest(const FAetherAccountId& Sender){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerRejectFriendRequest_Implementation(Id,Sender);else ServerRejectFriendRequest(Id,Sender);}
void AAetherNetworkPlayerController::RemoveFriend(const FAetherAccountId& Friend){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerRemoveFriend_Implementation(Id,Friend);else ServerRemoveFriend(Id,Friend);}
void AAetherNetworkPlayerController::BlockAccount(const FAetherAccountId& Target){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerBlockAccount_Implementation(Id,Target);else ServerBlockAccount(Id,Target);}
void AAetherNetworkPlayerController::UnblockAccount(const FAetherAccountId& Target){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerUnblockAccount_Implementation(Id,Target);else ServerUnblockAccount(Id,Target);}
void AAetherNetworkPlayerController::CreateParty(){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerCreateParty_Implementation(Id);else ServerCreateParty(Id);}
void AAetherNetworkPlayerController::InviteToParty(const FAetherAccountId& Target){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerInviteToParty_Implementation(Id,Target);else ServerInviteToParty(Id,Target);}
void AAetherNetworkPlayerController::AcceptPartyInvite(const FAetherSocialPartyId& PartyId){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerAcceptPartyInvite_Implementation(Id,PartyId);else ServerAcceptPartyInvite(Id,PartyId);}
void AAetherNetworkPlayerController::LeaveParty(){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerLeaveParty_Implementation(Id);else ServerLeaveParty(Id);}
void AAetherNetworkPlayerController::KickFromParty(const FAetherCharacterId& Target){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerKickFromParty_Implementation(Id,Target);else ServerKickFromParty(Id,Target);}
void AAetherNetworkPlayerController::CreateGuild(const FString& Name){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerCreateGuild_Implementation(Id,Name);else ServerCreateGuild(Id,Name);}
void AAetherNetworkPlayerController::InviteToGuild(const FAetherAccountId& Target){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerInviteToGuild_Implementation(Id,Target);else ServerInviteToGuild(Id,Target);}
void AAetherNetworkPlayerController::AcceptGuildInvite(const FAetherGuildId& GuildId){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerAcceptGuildInvite_Implementation(Id,GuildId);else ServerAcceptGuildInvite(Id,GuildId);}
void AAetherNetworkPlayerController::LeaveGuild(){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerLeaveGuild_Implementation(Id);else ServerLeaveGuild(Id);}
void AAetherNetworkPlayerController::SetGuildRole(const FAetherCharacterId& Target,EAetherGuildRole Role){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerSetGuildRole_Implementation(Id,Target,Role);else ServerSetGuildRole(Id,Target,Role);}
void AAetherNetworkPlayerController::SendChat(EAetherSocialChannel Channel,const FAetherAccountId& Target,const FString& Message){const uint32 Id=NextSocialRequestId++;if(HasAuthority())ServerSendChat_Implementation(Id,Channel,Target,Message);else ServerSendChat(Id,Channel,Target,Message);}

#define AETHER_SOCIAL_GUARD(Id) if((Id)==0||(Id)<=LastProcessedSocialRequestId){return;} if(!AuthorizeSecurityRequest((Id),EAetherSecurityAction::Social)){return;} LastProcessedSocialRequestId=(Id)

void AAetherNetworkPlayerController::ServerRequestFriends_Implementation(uint32 Id)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AETHER_SOCIAL_GUARD(Id);
    TArray<FAetherSocialRelation> Friends;
    UAetherSocialSubsystem* Social=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;
    if(Social&&bAccountAuthenticated)Social->GetFriends(AuthenticatedAccountId,Friends);
    ClientReceiveFriends(Id,Friends);
}
void AAetherNetworkPlayerController::ServerSendFriendRequest_Implementation(uint32 Id,const FAetherAccountId& Target){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::NotAuthenticated;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&bAccountAuthenticated)S->SendFriendRequest(AuthenticatedAccountId,Target,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerAcceptFriendRequest_Implementation(uint32 Id,const FAetherAccountId& Sender){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::NotAuthenticated;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&bAccountAuthenticated)S->AcceptFriendRequest(AuthenticatedAccountId,Sender,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerRejectFriendRequest_Implementation(uint32 Id,const FAetherAccountId& Sender){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::NotAuthenticated;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&bAccountAuthenticated)S->RejectFriendRequest(AuthenticatedAccountId,Sender,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerRemoveFriend_Implementation(uint32 Id,const FAetherAccountId& Friend){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::NotAuthenticated;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&bAccountAuthenticated)S->RemoveFriend(AuthenticatedAccountId,Friend,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerBlockAccount_Implementation(uint32 Id,const FAetherAccountId& Target){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::NotAuthenticated;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&bAccountAuthenticated)S->BlockAccount(AuthenticatedAccountId,Target,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerUnblockAccount_Implementation(uint32 Id,const FAetherAccountId& Target){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::NotAuthenticated;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&bAccountAuthenticated)S->UnblockAccount(AuthenticatedAccountId,Target,O);ClientReceiveSocialOperation(Id,O);}

void AAetherNetworkPlayerController::ServerCreateParty_Implementation(uint32 Id){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->CreateParty(C,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerInviteToParty_Implementation(uint32 Id,const FAetherAccountId& Target){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->InviteToParty(C,Target,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerAcceptPartyInvite_Implementation(uint32 Id,const FAetherSocialPartyId& PartyId){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->AcceptPartyInvite(C,PartyId,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerLeaveParty_Implementation(uint32 Id){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->LeaveParty(C,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerKickFromParty_Implementation(uint32 Id,const FAetherCharacterId& Target){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->KickFromParty(C,Target,O);ClientReceiveSocialOperation(Id,O);}

void AAetherNetworkPlayerController::ServerCreateGuild_Implementation(uint32 Id,const FString& Name){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->CreateGuild(C,Name,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerInviteToGuild_Implementation(uint32 Id,const FAetherAccountId& Target){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->InviteToGuild(C,Target,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerAcceptGuildInvite_Implementation(uint32 Id,const FAetherGuildId& GuildId){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->AcceptGuildInvite(C,GuildId,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerLeaveGuild_Implementation(uint32 Id){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->LeaveGuild(C,O);ClientReceiveSocialOperation(Id,O);}
void AAetherNetworkPlayerController::ServerSetGuildRole_Implementation(uint32 Id,const FAetherCharacterId& Target,EAetherGuildRole Role){AETHER_SOCIAL_GUARD(Id);FAetherSocialOperation O;O.Result=EAetherSocialResult::CharacterRequired;FAetherCharacterRecord C;UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;if(S&&GetControllerCharacter(this,C))S->SetGuildRole(C,Target,Role,O);ClientReceiveSocialOperation(Id,O);}

void AAetherNetworkPlayerController::ServerSendChat_Implementation(uint32 Id,EAetherSocialChannel Channel,const FAetherAccountId& Target,const FString& Message)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AETHER_SOCIAL_GUARD(Id);
    FAetherSocialResult Result=EAetherSocialResult::InvalidRequest;
    FAetherChatMessage Chat;
    FAetherCharacterRecord C;
    UAetherSocialSubsystem* S=GetGameInstance()?GetGameInstance()->GetSubsystem<UAetherSocialSubsystem>():nullptr;
    if(S&&GetControllerCharacter(this,C))S->ValidateChat(C,Channel,Target,Message,GetServerTimeSeconds(),Chat,Result);
    if(Result==EAetherSocialResult::Accepted)
    {
        if(UWorld* World=GetWorld())
        {
            for(FConstPlayerControllerIterator It=World->GetPlayerControllerIterator(); It; ++It)
            {
                AAetherNetworkPlayerController* Recipient=Cast<AAetherNetworkPlayerController>(It->Get());
                if(!Recipient || !Recipient->IsAccountAuthenticated()) continue;

                bool bDeliver=false;
                if(Channel==EAetherSocialChannel::Whisper)
                {
                    bDeliver=Recipient->GetAuthenticatedAccountId()==Target;
                }
                else if(Channel==EAetherSocialChannel::Party)
                {
                    FAetherCharacterRecord RecipientCharacter;
                    if(GetControllerCharacter(Recipient,RecipientCharacter))
                    {
                        FAetherPartyState Party;
                        bDeliver=S->GetPartyForCharacter(C.CharacterId,Party);
                        if(bDeliver)
                        {
                            bDeliver=false;
                            for(const FAetherPartyMember& Member:Party.Members)
                            {
                                if(Member.AccountId==RecipientCharacter.AccountId){bDeliver=true;break;}
                            }
                        }
                    }
                }
                else if(Channel==EAetherSocialChannel::Guild)
                {
                    FAetherCharacterRecord RecipientCharacter;
                    if(GetControllerCharacter(Recipient,RecipientCharacter))
                    {
                        FAetherGuildState Guild;
                        bDeliver=S->GetGuildForCharacter(C.CharacterId,Guild);
                        if(bDeliver)
                        {
                            bDeliver=false;
                            for(const FAetherGuildMember& Member:Guild.Members)
                            {
                                if(Member.AccountId==RecipientCharacter.AccountId){bDeliver=true;break;}
                            }
                        }
                    }
                }
                else if(Channel==EAetherSocialChannel::Local)
                {
                    const APawn* SenderPawn=GetPawn();
                    const APawn* RecipientPawn=Recipient->GetPawn();
                    bDeliver=SenderPawn && RecipientPawn
                        && FVector::DistSquared(SenderPawn->GetActorLocation(),RecipientPawn->GetActorLocation()) <= FMath::Square(2000.0f);
                }
                else
                {
                    bDeliver=true;
                }

                if(bDeliver) Recipient->ClientReceiveChat(Id,Chat);
            }
        }
    }
    else
    {
        FAetherSocialOperation O;
        O.Result=Result;
        ClientReceiveSocialOperation(Id,O);
    }
}

void AAetherNetworkPlayerController::ClientReceiveSocialOperation_Implementation(uint32 Id,const FAetherSocialOperation& Operation){OnSocialOperation.Broadcast(Operation);}
void AAetherNetworkPlayerController::ClientReceiveFriends_Implementation(uint32 Id,const TArray<FAetherSocialRelation>& Friends){OnFriends.Broadcast(Friends);}
void AAetherNetworkPlayerController::ClientReceiveChat_Implementation(uint32 Id,const FAetherChatMessage& Message){OnChatMessage.Broadcast(Message);}

#undef AETHER_SOCIAL_GUARD

void AAetherNetworkPlayerController::RequestWallet()
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    const uint32 Id = NextEconomyRequestId++;
    if (HasAuthority()) ServerRequestWallet_Implementation(Id); else ServerRequestWallet(Id);
}

void AAetherNetworkPlayerController::BuyItem(const FString& ShopId, const FAetherItemDefinitionId& ItemDefinitionId, int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    const uint32 Id = NextEconomyRequestId++;
    if (HasAuthority()) ServerBuyItem_Implementation(Id, ShopId, ItemDefinitionId, Quantity); else ServerBuyItem(Id, ShopId, ItemDefinitionId, Quantity);
}

void AAetherNetworkPlayerController::SellItem(const FString& ShopId, const FAetherItemInstanceId& InstanceId, int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    const uint32 Id = NextEconomyRequestId++;
    if (HasAuthority()) ServerSellItem_Implementation(Id, ShopId, InstanceId, Quantity); else ServerSellItem(Id, ShopId, InstanceId, Quantity);
}

void AAetherNetworkPlayerController::CraftItem(const FString& RecipeId, int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    const uint32 Id = NextEconomyRequestId++;
    if (HasAuthority()) ServerCraftItem_Implementation(Id, RecipeId, Quantity); else ServerCraftItem(Id, RecipeId, Quantity);
}

#define AETHER_ECONOMY_GUARD(Id) if((Id)==0||(Id)<=LastProcessedEconomyRequestId){return;} LastProcessedEconomyRequestId=(Id)

void AAetherNetworkPlayerController::ServerRequestWallet_Implementation(uint32 Id)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AETHER_ECONOMY_GUARD(Id);
    FAetherEconomyTransaction Transaction;
    Transaction.Result = EAetherEconomyResult::NotOwned;
    UAetherEconomySubsystem* Economy = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherEconomySubsystem>() : nullptr;
    FAetherCharacterRecord Character;
    if (Economy && GetControllerCharacter(this, Character))
    {
        Transaction.CharacterId = Character.CharacterId;
        Transaction.Result = EAetherEconomyResult::Accepted;
        Transaction.Currency = EAetherCurrency::Gold;
        Transaction.BalanceAfter = Economy->GetBalance(Character.CharacterId, EAetherCurrency::Gold);
    }
    ClientReceiveEconomy(Id, Transaction);
}

void AAetherNetworkPlayerController::ServerBuyItem_Implementation(uint32 Id, const FString& ShopId, const FAetherItemDefinitionId& ItemDefinitionId, int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AETHER_ECONOMY_GUARD(Id);
    FAetherEconomyTransaction Transaction;
    Transaction.Result = EAetherEconomyResult::NotOwned;
    FAetherCharacterRecord Character;
    UAetherEconomySubsystem* Economy = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherEconomySubsystem>() : nullptr;
    if (Economy && GetControllerCharacter(this, Character))
        Economy->Buy(Character.CharacterId, ShopId, ItemDefinitionId, Quantity, Transaction);
    ClientReceiveEconomy(Id, Transaction);
}

void AAetherNetworkPlayerController::ServerSellItem_Implementation(uint32 Id, const FString& ShopId, const FAetherItemInstanceId& InstanceId, int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AETHER_ECONOMY_GUARD(Id);
    FAetherEconomyTransaction Transaction;
    Transaction.Result = EAetherEconomyResult::NotOwned;
    FAetherCharacterRecord Character;
    UAetherEconomySubsystem* Economy = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherEconomySubsystem>() : nullptr;
    if (Economy && GetControllerCharacter(this, Character))
        Economy->Sell(Character.CharacterId, ShopId, InstanceId, Quantity, Transaction);
    ClientReceiveEconomy(Id, Transaction);
}

void AAetherNetworkPlayerController::ServerCraftItem_Implementation(uint32 Id, const FString& RecipeId, int32 Quantity)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AETHER_ECONOMY_GUARD(Id);
    FAetherEconomyTransaction Transaction;
    Transaction.Result = EAetherEconomyResult::NotOwned;
    FAetherCharacterRecord Character;
    UAetherEconomySubsystem* Economy = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherEconomySubsystem>() : nullptr;
    if (Economy && GetControllerCharacter(this, Character))
        Economy->Craft(Character.CharacterId, RecipeId, Quantity, Character.Level, Transaction);
    ClientReceiveEconomy(Id, Transaction);
}

void AAetherNetworkPlayerController::ClientReceiveEconomy_Implementation(uint32 Id, const FAetherEconomyTransaction& Transaction)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    OnEconomyTransaction.Broadcast(Transaction);
}

#undef AETHER_ECONOMY_GUARD

void AAetherNetworkPlayerController::AllocateStatPoints(EAetherCharacterStat Stat, int32 Amount)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    const uint32 RequestId = NextProgressionRequestId++;
    if (HasAuthority())
    {
        ServerAllocateStatPoints_Implementation(RequestId, Stat, Amount);
        return;
    }
    ServerAllocateStatPoints(RequestId, Stat, Amount);
}

void AAetherNetworkPlayerController::ServerAllocateStatPoints_Implementation(
    uint32 RequestId,
    EAetherCharacterStat Stat,
    int32 Amount)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Progression))
    {
        return;
    }
    if (RequestId == 0 || RequestId <= LastProcessedProgressionRequestId)
    {
        return;
    }

    FAetherProgressionResult Result;
    Result.Result = EAetherProgressionResult::NotOwned;

    const AAetherCharacterPlayerState* StateBefore = GetPlayerState<AAetherCharacterPlayerState>();
    const FAetherCharacterId CharacterId = StateBefore ? StateBefore->GetCharacterId() : FAetherCharacterId();

    UAetherProgressionSubsystem* Progression = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherProgressionSubsystem>()
        : nullptr;

    const FAetherAccountId AccountId = GetAuthenticatedAccountId();
    bool bAccepted = false;

    if (Progression && AccountId.IsValid() && CharacterId.IsValid())
    {
        bAccepted = Progression->AllocateStatPoints(AccountId, CharacterId, Stat, Amount, Result);
    }

    LastProcessedProgressionRequestId = RequestId;

    if (bAccepted)
    {
        if (AAetherCharacterPlayerState* State = GetPlayerState<AAetherCharacterPlayerState>())
        {
            FAetherCharacterRecord Character;
            UAetherCharacterSubsystem* Characters = GetGameInstance()
                ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
                : nullptr;

            if (Characters && Characters->FindCharacter(CharacterId, Character))
            {
                State->SetCharacterIdentity(Character);
            }
        }
    }

    ClientReceiveProgression(RequestId, Result);
}

void AAetherNetworkPlayerController::ClientReceiveProgression_Implementation(
    uint32 RequestId,
    const FAetherProgressionResult& Result)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    OnProgression.Broadcast(Result);
}

bool AAetherNetworkPlayerController::AuthorizeSecurityRequest(uint32 RequestId, EAetherSecurityAction Action) const
{
    if (!GetGameInstance())
    {
        return false;
    }

    UAetherSecuritySubsystem* Security =
        GetGameInstance()->GetSubsystem<UAetherSecuritySubsystem>();

    if (!Security)
    {
        return false;
    }

    return Security->GetService().AuthorizeRequest(
        GetUniqueID(),
        RequestId,
        Action,
        bAccountAuthenticated,
        GetServerTimeSeconds()) == EAetherSecurityResult::Accepted;
}

bool AAetherNetworkPlayerController::ValidateRequest(const FAetherNetworkRequest& Request) const
{
    const FAetherProtocolVersion ServerProtocol = FAetherProtocolVersion::Current();

    if (!Request.ProtocolVersion.IsCompatibleWith(ServerProtocol))
    {
        return false;
    }

    if (Request.RequestId == 0 || Request.RequestId <= LastProcessedRequestId)
    {
        return false;
    }

    return true;
}

bool AAetherNetworkPlayerController::ValidateAccountRequestId(uint32 RequestId) const
{
    return RequestId != 0 && RequestId > LastProcessedAccountRequestId;
}

double AAetherNetworkPlayerController::GetServerTimeSeconds() const
{
    return FPlatformTime::Seconds();
}

void AAetherNetworkPlayerController::ApplyAuthenticatedSession(const FAetherAuthenticationResponse& Response)
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AuthenticatedAccountId = Response.AccountId;
    SessionId = Response.SessionId;
    bAccountAuthenticated = true;
}

void AAetherNetworkPlayerController::ClearAuthenticatedSession()
{
    if (!AuthorizeSecurityRequest(RequestId, EAetherSecurityAction::Social))
    {
        return;
    }
    AuthenticatedAccountId = FAetherAccountId();
    SessionId = FAetherSessionId();
    bAccountAuthenticated = false;
}
