#pragma once

#include "CoreMinimal.h"
#include "Social/AetherSocialTypes.h"

class FAetherSocialService
{
public:
    FAetherSocialService();

    const FAetherSocialConfig& GetConfig() const;
    bool SetConfig(const FAetherSocialConfig& NewConfig);

    bool SendFriendRequest(const FAetherAccountId& Sender, const FAetherAccountId& Target, FAetherSocialOperation& Out);
    bool AcceptFriendRequest(const FAetherAccountId& Receiver, const FAetherAccountId& Sender, FAetherSocialOperation& Out);
    bool RejectFriendRequest(const FAetherAccountId& Receiver, const FAetherAccountId& Sender, FAetherSocialOperation& Out);
    bool RemoveFriend(const FAetherAccountId& Account, const FAetherAccountId& Friend, FAetherSocialOperation& Out);
    bool BlockAccount(const FAetherAccountId& Account, const FAetherAccountId& Target, FAetherSocialOperation& Out);
    bool UnblockAccount(const FAetherAccountId& Account, const FAetherAccountId& Target, FAetherSocialOperation& Out);
    bool GetFriends(const FAetherAccountId& Account, TArray<FAetherSocialRelation>& OutFriends) const;

    bool CreateParty(const FAetherCharacterRecord& Character, FAetherSocialOperation& Out);
    bool InviteToParty(const FAetherCharacterRecord& Sender, const FAetherAccountId& TargetAccount, FAetherSocialOperation& Out);
    bool AcceptPartyInvite(const FAetherCharacterRecord& Character, const FAetherSocialPartyId& PartyId, FAetherSocialOperation& Out);
    bool LeaveParty(const FAetherCharacterRecord& Character, FAetherSocialOperation& Out);
    bool KickFromParty(const FAetherCharacterRecord& Leader, const FAetherCharacterId& TargetCharacter, FAetherSocialOperation& Out);
    bool GetPartyForCharacter(const FAetherCharacterId& CharacterId, FAetherPartyState& OutParty) const;

    bool CreateGuild(const FAetherCharacterRecord& Character, const FString& Name, FAetherSocialOperation& Out);
    bool InviteToGuild(const FAetherCharacterRecord& Leader, const FAetherAccountId& TargetAccount, FAetherSocialOperation& Out);
    bool AcceptGuildInvite(const FAetherCharacterRecord& Character, const FAetherGuildId& GuildId, FAetherSocialOperation& Out);
    bool LeaveGuild(const FAetherCharacterRecord& Character, FAetherSocialOperation& Out);
    bool SetGuildRole(const FAetherCharacterRecord& Leader, const FAetherCharacterId& TargetCharacter, EAetherGuildRole Role, FAetherSocialOperation& Out);
    bool GetGuildForCharacter(const FAetherCharacterId& CharacterId, FAetherGuildState& OutGuild) const;

    bool ValidateChat(
        const FAetherCharacterRecord& Character,
        EAetherSocialChannel Channel,
        const FAetherAccountId& TargetAccount,
        const FString& Message,
        double ServerTimeSeconds,
        FAetherChatMessage& OutMessage,
        EAetherSocialResult& OutResult);

    void ClearCharacterState(const FAetherCharacterId& CharacterId);
    void ClearAccountState(const FAetherAccountId& AccountId);

private:
    static bool IsConfigValid(const FAetherSocialConfig& Config);
    static bool IsCharacterUsable(const FAetherCharacterRecord& Character);
    static FString NormalizeGuildName(const FString& Name);
    static FString NormalizeChatMessage(const FString& Message);

    bool AreFriends(const FAetherAccountId& A, const FAetherAccountId& B) const;
    bool IsBlocked(const FAetherAccountId& A, const FAetherAccountId& B) const;
    bool IsInParty(const FAetherCharacterId& CharacterId, FAetherSocialPartyId* OutPartyId = nullptr) const;
    bool IsInGuild(const FAetherCharacterId& CharacterId, FAetherGuildId* OutGuildId = nullptr) const;
    bool IsAccountInParty(const FAetherAccountId& AccountId, FAetherSocialPartyId* OutPartyId = nullptr) const;
    bool IsAccountInGuild(const FAetherAccountId& AccountId, FAetherGuildId* OutGuildId = nullptr) const;
    bool GetPartyMember(const FAetherSocialPartyId& PartyId, const FAetherCharacterId& CharacterId, FAetherPartyMember& OutMember) const;
    bool GetGuildMember(const FAetherGuildId& GuildId, const FAetherCharacterId& CharacterId, FAetherGuildMember& OutMember) const;
    void BuildFriends(const FAetherAccountId& AccountId, TArray<FAetherSocialRelation>& OutFriends) const;

    FAetherSocialConfig Config;
    TMap<FAetherAccountId, TMap<FAetherAccountId, EAetherFriendState>> FriendRelations;
    TMap<FAetherSocialPartyId, FAetherPartyState> Parties;
    TMap<FAetherCharacterId, FAetherSocialPartyId> PartyByCharacter;
    TMap<FAetherAccountId, FAetherSocialPartyId> PartyByAccount;
    TMap<FAetherSocialPartyId, TSet<FAetherAccountId>> PendingPartyInvites;
    TMap<FAetherGuildId, FAetherGuildState> Guilds;
    TMap<FAetherCharacterId, FAetherGuildId> GuildByCharacter;
    TMap<FAetherAccountId, FAetherGuildId> GuildByAccount;
    TMap<FAetherGuildId, TSet<FAetherAccountId>> PendingGuildInvites;
    TMap<FAetherAccountId, double> LastChatAt;
};
