#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Social/AetherSocialService.h"
#include "AetherSocialSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherSocialSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool SendFriendRequest(const FAetherAccountId& Sender,const FAetherAccountId& Target,FAetherSocialOperation& Out);
    bool AcceptFriendRequest(const FAetherAccountId& Receiver,const FAetherAccountId& Sender,FAetherSocialOperation& Out);
    bool RejectFriendRequest(const FAetherAccountId& Receiver,const FAetherAccountId& Sender,FAetherSocialOperation& Out);
    bool RemoveFriend(const FAetherAccountId& Account,const FAetherAccountId& Friend,FAetherSocialOperation& Out);
    bool BlockAccount(const FAetherAccountId& Account,const FAetherAccountId& Target,FAetherSocialOperation& Out);
    bool UnblockAccount(const FAetherAccountId& Account,const FAetherAccountId& Target,FAetherSocialOperation& Out);
    bool GetFriends(const FAetherAccountId& Account,TArray<FAetherSocialRelation>& Out) const;

    bool CreateParty(const FAetherCharacterRecord& Character,FAetherSocialOperation& Out);
    bool InviteToParty(const FAetherCharacterRecord& Character,const FAetherAccountId& Target,FAetherSocialOperation& Out);
    bool AcceptPartyInvite(const FAetherCharacterRecord& Character,const FAetherSocialPartyId& PartyId,FAetherSocialOperation& Out);
    bool LeaveParty(const FAetherCharacterRecord& Character,FAetherSocialOperation& Out);
    bool KickFromParty(const FAetherCharacterRecord& Character,const FAetherCharacterId& Target,FAetherSocialOperation& Out);
    bool GetPartyForCharacter(const FAetherCharacterId& CharacterId,FAetherPartyState& Out) const;

    bool CreateGuild(const FAetherCharacterRecord& Character,const FString& Name,FAetherSocialOperation& Out);
    bool InviteToGuild(const FAetherCharacterRecord& Character,const FAetherAccountId& Target,FAetherSocialOperation& Out);
    bool AcceptGuildInvite(const FAetherCharacterRecord& Character,const FAetherGuildId& GuildId,FAetherSocialOperation& Out);
    bool LeaveGuild(const FAetherCharacterRecord& Character,FAetherSocialOperation& Out);
    bool SetGuildRole(const FAetherCharacterRecord& Character,const FAetherCharacterId& Target,EAetherGuildRole Role,FAetherSocialOperation& Out);
    bool GetGuildForCharacter(const FAetherCharacterId& CharacterId,FAetherGuildState& Out) const;

    bool ValidateChat(const FAetherCharacterRecord& Character,EAetherSocialChannel Channel,const FAetherAccountId& Target,const FString& Message,double ServerTime,FAetherChatMessage& OutMessage,EAetherSocialResult& OutResult);

    void ClearCharacterState(const FAetherCharacterId& CharacterId);
    void ClearAccountState(const FAetherAccountId& AccountId);

private:
    FAetherSocialService SocialService;
};
