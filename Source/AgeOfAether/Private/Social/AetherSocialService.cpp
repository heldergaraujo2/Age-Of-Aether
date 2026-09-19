#include "Social/AetherSocialService.h"

FAetherSocialService::FAetherSocialService()
{
    SetConfig(Config);
}

const FAetherSocialConfig& FAetherSocialService::GetConfig() const { return Config; }

bool FAetherSocialService::SetConfig(const FAetherSocialConfig& NewConfig)
{
    if (!IsConfigValid(NewConfig)) return false;
    Config = NewConfig;
    return true;
}

bool FAetherSocialService::IsConfigValid(const FAetherSocialConfig& C)
{
    return C.MaxPartyMembers >= 2 && C.MaxGuildMembers >= 2 && C.MaxMessageLength > 0 && C.ChatCooldownSeconds >= 0.0f;
}

bool FAetherSocialService::IsCharacterUsable(const FAetherCharacterRecord& C)
{
    return C.CharacterId.IsValid() && C.AccountId.IsValid() && C.Status == EAetherCharacterStatus::Active;
}

FString FAetherSocialService::NormalizeGuildName(const FString& Name)
{
    return Name.TrimStartAndEnd().ToLower();
}

FString FAetherSocialService::NormalizeChatMessage(const FString& Message)
{
    return Message.TrimStartAndEnd();
}

bool FAetherSocialService::IsBlocked(const FAetherAccountId& A, const FAetherAccountId& B) const
{
    const TMap<FAetherAccountId, EAetherFriendState>* R = FriendRelations.Find(A);
    return R && R->FindRef(B) == EAetherFriendState::Blocked;
}

bool FAetherSocialService::AreFriends(const FAetherAccountId& A, const FAetherAccountId& B) const
{
    const TMap<FAetherAccountId, EAetherFriendState>* R = FriendRelations.Find(A);
    return R && R->FindRef(B) == EAetherFriendState::Accepted;
}

void FAetherSocialService::BuildFriends(const FAetherAccountId& AccountId, TArray<FAetherSocialRelation>& OutFriends) const
{
    OutFriends.Empty();
    const TMap<FAetherAccountId, EAetherFriendState>* R = FriendRelations.Find(AccountId);
    if (!R) return;
    for (const TPair<FAetherAccountId, EAetherFriendState>& Pair : *R)
    {
        FAetherSocialRelation Relation;
        Relation.AccountId = Pair.Key;
        Relation.State = Pair.Value;
        OutFriends.Add(Relation);
    }
    OutFriends.Sort([](const FAetherSocialRelation& A, const FAetherSocialRelation& B)
    {
        return A.AccountId.Value < B.AccountId.Value;
    });
}

bool FAetherSocialService::SendFriendRequest(const FAetherAccountId& Sender, const FAetherAccountId& Target, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    if (!Sender.IsValid() || !Target.IsValid()) { Out.Result = EAetherSocialResult::NotAuthenticated; return false; }
    if (Sender == Target) { Out.Result = EAetherSocialResult::CannotTargetSelf; return false; }
    if (IsBlocked(Sender, Target) || IsBlocked(Target, Sender)) { Out.Result = EAetherSocialResult::Blocked; return false; }

    TMap<FAetherAccountId, EAetherFriendState>& S = FriendRelations.FindOrAdd(Sender);
    TMap<FAetherAccountId, EAetherFriendState>& T = FriendRelations.FindOrAdd(Target);
    if (S.FindRef(Target) == EAetherFriendState::Accepted) { Out.Result = EAetherSocialResult::AlreadyFriends; return false; }
    if (S.FindRef(Target) == EAetherFriendState::PendingOutgoing || T.FindRef(Sender) == EAetherFriendState::PendingIncoming)
    {
        Out.Result = EAetherSocialResult::RequestPending; return false;
    }

    S.Add(Target, EAetherFriendState::PendingOutgoing);
    T.Add(Sender, EAetherFriendState::PendingIncoming);
    Out.Result = EAetherSocialResult::Accepted;
    BuildFriends(Sender, Out.Friends);
    return true;
}

bool FAetherSocialService::AcceptFriendRequest(const FAetherAccountId& Receiver, const FAetherAccountId& Sender, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    const TMap<FAetherAccountId, EAetherFriendState>* R = FriendRelations.Find(Receiver);
    if (!R || R->FindRef(Sender) != EAetherFriendState::PendingIncoming)
    {
        Out.Result = EAetherSocialResult::RequestNotFound; return false;
    }
    FriendRelations.FindOrAdd(Receiver).Add(Sender, EAetherFriendState::Accepted);
    FriendRelations.FindOrAdd(Sender).Add(Receiver, EAetherFriendState::Accepted);
    Out.Result = EAetherSocialResult::Accepted;
    BuildFriends(Receiver, Out.Friends);
    return true;
}

bool FAetherSocialService::RejectFriendRequest(const FAetherAccountId& Receiver, const FAetherAccountId& Sender, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    TMap<FAetherAccountId, EAetherFriendState>* R = FriendRelations.Find(Receiver);
    if (!R || R->FindRef(Sender) != EAetherFriendState::PendingIncoming)
    {
        Out.Result = EAetherSocialResult::RequestNotFound; return false;
    }
    R->Remove(Sender);
    FriendRelations.FindOrAdd(Sender).Remove(Receiver);
    Out.Result = EAetherSocialResult::Accepted;
    BuildFriends(Receiver, Out.Friends);
    return true;
}

bool FAetherSocialService::RemoveFriend(const FAetherAccountId& Account, const FAetherAccountId& Friend, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    if (!AreFriends(Account, Friend))
    {
        Out.Result = EAetherSocialResult::RequestNotFound; return false;
    }
    FriendRelations.FindOrAdd(Account).Remove(Friend);
    FriendRelations.FindOrAdd(Friend).Remove(Account);
    Out.Result = EAetherSocialResult::Accepted;
    BuildFriends(Account, Out.Friends);
    return true;
}

bool FAetherSocialService::BlockAccount(const FAetherAccountId& Account, const FAetherAccountId& Target, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    if (!Account.IsValid() || !Target.IsValid() || Account == Target)
    {
        Out.Result = EAetherSocialResult::InvalidRequest; return false;
    }
    FriendRelations.FindOrAdd(Account).Add(Target, EAetherFriendState::Blocked);
    FriendRelations.FindOrAdd(Target).Remove(Account);
    Out.Result = EAetherSocialResult::Accepted;
    BuildFriends(Account, Out.Friends);
    return true;
}

bool FAetherSocialService::UnblockAccount(const FAetherAccountId& Account, const FAetherAccountId& Target, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    TMap<FAetherAccountId, EAetherFriendState>* R = FriendRelations.Find(Account);
    if (!R || R->FindRef(Target) != EAetherFriendState::Blocked)
    {
        Out.Result = EAetherSocialResult::RequestNotFound; return false;
    }
    R->Remove(Target);
    Out.Result = EAetherSocialResult::Accepted;
    BuildFriends(Account, Out.Friends);
    return true;
}

bool FAetherSocialService::GetFriends(const FAetherAccountId& Account, TArray<FAetherSocialRelation>& OutFriends) const
{
    BuildFriends(Account, OutFriends);
    return true;
}

bool FAetherSocialService::IsInParty(const FAetherCharacterId& CharacterId, FAetherSocialPartyId* OutPartyId) const
{
    const FAetherSocialPartyId* Id = PartyByCharacter.Find(CharacterId);
    if (!Id) return false;
    if (OutPartyId) *OutPartyId = *Id;
    return true;
}

bool FAetherSocialService::IsAccountInParty(const FAetherAccountId& AccountId, FAetherSocialPartyId* OutPartyId) const
{
    const FAetherSocialPartyId* Id = PartyByAccount.Find(AccountId);
    if (!Id) return false;
    if (OutPartyId) *OutPartyId = *Id;
    return true;
}

bool FAetherSocialService::GetPartyMember(const FAetherSocialPartyId& PartyId, const FAetherCharacterId& CharacterId, FAetherPartyMember& OutMember) const
{
    const FAetherSocialPartyId* Existing = PartyByCharacter.Find(CharacterId);
    if (!Existing || !(*Existing == PartyId)) return false;
    const FAetherPartyState* Party = Parties.Find(PartyId);
    if (!Party) return false;
    for (const FAetherPartyMember& Member : Party->Members)
    {
        if (Member.CharacterId == CharacterId) { OutMember = Member; return true; }
    }
    return false;
}

bool FAetherSocialService::CreateParty(const FAetherCharacterRecord& Character, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    if (!IsCharacterUsable(Character)) { Out.Result = EAetherSocialResult::CharacterRequired; return false; }
    if (IsAccountInParty(Character.AccountId)) { Out.Result = EAetherSocialResult::AlreadyInParty; return false; }

    FAetherSocialPartyId PartyId = FAetherSocialPartyId::NewId();
    FAetherPartyState Party;
    Party.PartyId = PartyId;
    FAetherPartyMember Member;
    Member.CharacterId = Character.CharacterId;
    Member.AccountId = Character.AccountId;
    Member.Role = EAetherPartyRole::Leader;
    Party.Members.Add(Member);
    Parties.Add(PartyId, Party);
    PartyByCharacter.Add(Character.CharacterId, PartyId);
    PartyByAccount.Add(Character.AccountId, PartyId);
    Out.Result = EAetherSocialResult::Accepted;
    Out.Party = Party;
    return true;
}

bool FAetherSocialService::InviteToParty(const FAetherCharacterRecord& Sender, const FAetherAccountId& TargetAccount, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    FAetherSocialPartyId PartyId;
    if (!GetPartyForCharacter(Sender.CharacterId, Out.Party)) { Out.Result = EAetherSocialResult::NotPartyMember; return false; }
    PartyId = Out.Party.PartyId;
    FAetherPartyMember SenderMember;
    if (!GetPartyMember(PartyId, Sender.CharacterId, SenderMember) || SenderMember.Role != EAetherPartyRole::Leader)
    {
        Out.Result = EAetherSocialResult::NotPartyLeader; return false;
    }
    if (Sender.AccountId == TargetAccount) { Out.Result = EAetherSocialResult::CannotTargetSelf; return false; }
    if (IsAccountInParty(TargetAccount)) { Out.Result = EAetherSocialResult::AlreadyInParty; return false; }
    if (Out.Party.Members.Num() >= Config.MaxPartyMembers) { Out.Result = EAetherSocialResult::PartyFull; return false; }
    if (IsBlocked(Sender.AccountId, TargetAccount) || IsBlocked(TargetAccount, Sender.AccountId)) { Out.Result = EAetherSocialResult::Blocked; return false; }
    PendingPartyInvites.FindOrAdd(PartyId).Add(TargetAccount);
    Out.Result = EAetherSocialResult::Accepted;
    return true;
}

bool FAetherSocialService::AcceptPartyInvite(const FAetherCharacterRecord& Character, const FAetherSocialPartyId& PartyId, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    if (!IsCharacterUsable(Character)) { Out.Result = EAetherSocialResult::CharacterRequired; return false; }
    if (IsAccountInParty(Character.AccountId)) { Out.Result = EAetherSocialResult::AlreadyInParty; return false; }
    TSet<FAetherAccountId>* Invites = PendingPartyInvites.Find(PartyId);
    FAetherPartyState* Party = Parties.Find(PartyId);
    if (!Invites || !Invites->Contains(Character.AccountId) || !Party) { Out.Result = EAetherSocialResult::NotInvited; return false; }
    if (Party->Members.Num() >= Config.MaxPartyMembers) { Out.Result = EAetherSocialResult::PartyFull; return false; }
    FAetherPartyMember Member;
    Member.CharacterId = Character.CharacterId;
    Member.AccountId = Character.AccountId;
    Member.Role = EAetherPartyRole::Member;
    Party->Members.Add(Member);
    PartyByCharacter.Add(Character.CharacterId, PartyId);
    PartyByAccount.Add(Character.AccountId, PartyId);
    Invites->Remove(Character.AccountId);
    Out.Result = EAetherSocialResult::Accepted;
    Out.Party = *Party;
    return true;
}

bool FAetherSocialService::LeaveParty(const FAetherCharacterRecord& Character, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    FAetherSocialPartyId PartyId;
    if (!IsInParty(Character.CharacterId, &PartyId)) { Out.Result = EAetherSocialResult::NotPartyMember; return false; }
    FAetherPartyState* Party = Parties.Find(PartyId);
    if (!Party) { Out.Result = EAetherSocialResult::PartyNotFound; return false; }

    int32 Index = INDEX_NONE;
    for (int32 I=0; I<Party->Members.Num(); ++I) if (Party->Members[I].CharacterId == Character.CharacterId) { Index=I; break; }
    if (Index == INDEX_NONE) { Out.Result = EAetherSocialResult::NotPartyMember; return false; }

    const bool bLeader = Party->Members[Index].Role == EAetherPartyRole::Leader;
    Party->Members.RemoveAt(Index);
    PartyByCharacter.Remove(Character.CharacterId);
    PartyByAccount.Remove(Character.AccountId);

    if (Party->Members.Num() == 0)
    {
        PendingPartyInvites.Remove(PartyId);
        Parties.Remove(PartyId);
        Out.Result = EAetherSocialResult::Accepted;
        return true;
    }

    if (bLeader)
    {
        Party->Members[0].Role = EAetherPartyRole::Leader;
    }
    Out.Result = EAetherSocialResult::Accepted;
    Out.Party = *Party;
    return true;
}

bool FAetherSocialService::KickFromParty(const FAetherCharacterRecord& Leader, const FAetherCharacterId& TargetCharacter, FAetherSocialOperation& Out)
{
    Out = FAetherSocialOperation();
    FAetherSocialPartyId PartyId;
    if (!IsInParty(Leader.CharacterId, &PartyId)) { Out.Result = EAetherSocialResult::NotPartyMember; return false; }
    FAetherPartyState* Party = Parties.Find(PartyId);
    if (!Party) { Out.Result = EAetherSocialResult::PartyNotFound; return false; }
    FAetherPartyMember LeaderMember;
    if (!GetPartyMember(PartyId, Leader.CharacterId, LeaderMember) || LeaderMember.Role != EAetherPartyRole::Leader) { Out.Result = EAetherSocialResult::NotPartyLeader; return false; }
    int32 Index=INDEX_NONE;
    for(int32 I=0;I<Party->Members.Num();++I) if(Party->Members[I].CharacterId==TargetCharacter){Index=I;break;}
    if(Index==INDEX_NONE){Out.Result=EAetherSocialResult::NotPartyMember;return false;}
    if(TargetCharacter==Leader.CharacterId){Out.Result=EAetherSocialResult::InvalidRequest;return false;}
    const FAetherAccountId TargetAccount=Party->Members[Index].AccountId;
    Party->Members.RemoveAt(Index);
    PartyByCharacter.Remove(TargetCharacter);
    PartyByAccount.Remove(TargetAccount);
    Out.Result=EAetherSocialResult::Accepted;
    Out.Party=*Party;
    return true;
}

bool FAetherSocialService::GetPartyForCharacter(const FAetherCharacterId& CharacterId, FAetherPartyState& OutParty) const
{
    FAetherSocialPartyId Id;
    if (!IsInParty(CharacterId,&Id)) return false;
    const FAetherPartyState* Party=Parties.Find(Id);
    if(!Party)return false;
    OutParty=*Party;
    return true;
}

bool FAetherSocialService::IsInGuild(const FAetherCharacterId& CharacterId, FAetherGuildId* OutGuildId) const
{
    const FAetherGuildId* Id=GuildByCharacter.Find(CharacterId);
    if(!Id)return false;
    if(OutGuildId)*OutGuildId=*Id;
    return true;
}

bool FAetherSocialService::IsAccountInGuild(const FAetherAccountId& AccountId, FAetherGuildId* OutGuildId) const
{
    const FAetherGuildId* Id=GuildByAccount.Find(AccountId);
    if(!Id)return false;
    if(OutGuildId)*OutGuildId=*Id;
    return true;
}

bool FAetherSocialService::GetGuildMember(const FAetherGuildId& GuildId,const FAetherCharacterId& CharacterId,FAetherGuildMember& OutMember) const
{
    const FAetherGuildState* Guild=Guilds.Find(GuildId);
    if(!Guild)return false;
    for(const FAetherGuildMember& M:Guild->Members)if(M.CharacterId==CharacterId){OutMember=M;return true;}
    return false;
}

bool FAetherSocialService::CreateGuild(const FAetherCharacterRecord& Character,const FString& Name,FAetherSocialOperation& Out)
{
    Out=FAetherSocialOperation();
    if(!IsCharacterUsable(Character)){Out.Result=EAetherSocialResult::CharacterRequired;return false;}
    if(IsAccountInGuild(Character.AccountId)){Out.Result=EAetherSocialResult::AlreadyInGuild;return false;}
    const FString Normalized=NormalizeGuildName(Name);
    if(Normalized.Len()<3||Normalized.Len()>24){Out.Result=EAetherSocialResult::InvalidRequest;return false;}
    for(const TPair<FAetherGuildId,FAetherGuildState>& Pair:Guilds)if(Pair.Value.Name==Normalized){Out.Result=EAetherSocialResult::GuildNameUnavailable;return false;}

    FAetherGuildState Guild;
    Guild.GuildId=FAetherGuildId::NewId();
    Guild.Name=Normalized;
    FAetherGuildMember Member;
    Member.CharacterId=Character.CharacterId;
    Member.AccountId=Character.AccountId;
    Member.Role=EAetherGuildRole::Leader;
    Guild.Members.Add(Member);
    Guilds.Add(Guild.GuildId,Guild);
    GuildByCharacter.Add(Character.CharacterId,Guild.GuildId);
    GuildByAccount.Add(Character.AccountId,Guild.GuildId);
    Out.Result=EAetherSocialResult::Accepted;
    Out.Guild=Guild;
    return true;
}

bool FAetherSocialService::InviteToGuild(const FAetherCharacterRecord& Leader,const FAetherAccountId& TargetAccount,FAetherSocialOperation& Out)
{
    Out=FAetherSocialOperation();
    FAetherGuildId GuildId;
    if(!IsInGuild(Leader.CharacterId,&GuildId)){Out.Result=EAetherSocialResult::NotGuildMember;return false;}
    FAetherGuildState* Guild=Guilds.Find(GuildId);
    FAetherGuildMember M;
    if(!Guild||!GetGuildMember(GuildId,Leader.CharacterId,M)||M.Role!=EAetherGuildRole::Leader){Out.Result=EAetherSocialResult::NotGuildLeader;return false;}
    if(TargetAccount==Leader.AccountId){Out.Result=EAetherSocialResult::CannotTargetSelf;return false;}
    if(IsAccountInGuild(TargetAccount)){Out.Result=EAetherSocialResult::AlreadyInGuild;return false;}
    if(Guild->Members.Num()>=Config.MaxGuildMembers){Out.Result=EAetherSocialResult::GuildFull;return false;}
    PendingGuildInvites.FindOrAdd(GuildId).Add(TargetAccount);
    Out.Result=EAetherSocialResult::Accepted;
    Out.Guild=*Guild;
    return true;
}

bool FAetherSocialService::AcceptGuildInvite(const FAetherCharacterRecord& Character,const FAetherGuildId& GuildId,FAetherSocialOperation& Out)
{
    Out=FAetherSocialOperation();
    if(!IsCharacterUsable(Character)){Out.Result=EAetherSocialResult::CharacterRequired;return false;}
    if(IsAccountInGuild(Character.AccountId)){Out.Result=EAetherSocialResult::AlreadyInGuild;return false;}
    TSet<FAetherAccountId>* Invites=PendingGuildInvites.Find(GuildId);
    FAetherGuildState* Guild=Guilds.Find(GuildId);
    if(!Invites||!Invites->Contains(Character.AccountId)||!Guild){Out.Result=EAetherSocialResult::NotInvited;return false;}
    if(Guild->Members.Num()>=Config.MaxGuildMembers){Out.Result=EAetherSocialResult::GuildFull;return false;}
    FAetherGuildMember Member;
    Member.CharacterId=Character.CharacterId;Member.AccountId=Character.AccountId;Member.Role=EAetherGuildRole::Member;
    Guild->Members.Add(Member);
    GuildByCharacter.Add(Character.CharacterId,GuildId);
    GuildByAccount.Add(Character.AccountId,GuildId);
    Invites->Remove(Character.AccountId);
    Out.Result=EAetherSocialResult::Accepted;Out.Guild=*Guild;return true;
}

bool FAetherSocialService::LeaveGuild(const FAetherCharacterRecord& Character,FAetherSocialOperation& Out)
{
    Out=FAetherSocialOperation();
    FAetherGuildId GuildId;
    if(!IsInGuild(Character.CharacterId,&GuildId)){Out.Result=EAetherSocialResult::NotGuildMember;return false;}
    FAetherGuildState* Guild=Guilds.Find(GuildId);
    if(!Guild){Out.Result=EAetherSocialResult::GuildNotFound;return false;}
    int32 Index=INDEX_NONE;
    for(int32 I=0;I<Guild->Members.Num();++I)if(Guild->Members[I].CharacterId==Character.CharacterId){Index=I;break;}
    if(Index==INDEX_NONE){Out.Result=EAetherSocialResult::NotGuildMember;return false;}
    const bool bLeader=Guild->Members[Index].Role==EAetherGuildRole::Leader;
    if(bLeader&&Guild->Members.Num()>1){Out.Result=EAetherSocialResult::NotGuildLeader;return false;}
    Guild->Members.RemoveAt(Index);
    GuildByCharacter.Remove(Character.CharacterId);GuildByAccount.Remove(Character.AccountId);
    if(Guild->Members.Num()==0){PendingGuildInvites.Remove(GuildId);Guilds.Remove(GuildId);}
    Out.Result=EAetherSocialResult::Accepted;
    if(Guilds.Contains(GuildId))Out.Guild=*Guild;
    return true;
}

bool FAetherSocialService::SetGuildRole(const FAetherCharacterRecord& Leader,const FAetherCharacterId& TargetCharacter,EAetherGuildRole Role,FAetherSocialOperation& Out)
{
    Out=FAetherSocialOperation();
    FAetherGuildId GuildId;
    if(!IsInGuild(Leader.CharacterId,&GuildId)){Out.Result=EAetherSocialResult::NotGuildMember;return false;}
    FAetherGuildState* Guild=Guilds.Find(GuildId);
    FAetherGuildMember LeaderMember,TargetMember;
    if(!Guild||!GetGuildMember(GuildId,Leader.CharacterId,LeaderMember)||LeaderMember.Role!=EAetherGuildRole::Leader){Out.Result=EAetherSocialResult::NotGuildLeader;return false;}
    if(!GetGuildMember(GuildId,TargetCharacter,TargetMember)){Out.Result=EAetherSocialResult::NotGuildMember;return false;}
    if(TargetCharacter==Leader.CharacterId&&Role!=EAetherGuildRole::Leader){Out.Result=EAetherSocialResult::InvalidRequest;return false;}
    for(FAetherGuildMember& M:Guild->Members)if(M.CharacterId==TargetCharacter)M.Role=Role;
    Out.Result=EAetherSocialResult::Accepted;Out.Guild=*Guild;return true;
}

bool FAetherSocialService::GetGuildForCharacter(const FAetherCharacterId& CharacterId,FAetherGuildState& OutGuild) const
{
    FAetherGuildId Id;if(!IsInGuild(CharacterId,&Id))return false;
    const FAetherGuildState* Guild=Guilds.Find(Id);if(!Guild)return false;OutGuild=*Guild;return true;
}

bool FAetherSocialService::ValidateChat(const FAetherCharacterRecord& Character,EAetherSocialChannel Channel,const FAetherAccountId& TargetAccount,const FString& Message,double ServerTimeSeconds,FAetherChatMessage& OutMessage,EAetherSocialResult& OutResult)
{
    OutMessage=FAetherChatMessage();
    OutResult=EAetherSocialResult::InvalidRequest;
    if(!IsCharacterUsable(Character)){OutResult=EAetherSocialResult::CharacterRequired;return false;}
    const FString Clean=NormalizeChatMessage(Message);
    if(Clean.IsEmpty()||Clean.Len()>Config.MaxMessageLength){OutResult=EAetherSocialResult::InvalidMessage;return false;}
    if(Channel==EAetherSocialChannel::Whisper)
    {
        if(!TargetAccount.IsValid()||TargetAccount==Character.AccountId){OutResult=EAetherSocialResult::CannotTargetSelf;return false;}
        if(IsBlocked(Character.AccountId,TargetAccount)||IsBlocked(TargetAccount,Character.AccountId)){OutResult=EAetherSocialResult::Blocked;return false;}
    }
    if(Channel==EAetherSocialChannel::Party&&!IsInParty(Character.CharacterId)){OutResult=EAetherSocialResult::NotPartyMember;return false;}
    if(Channel==EAetherSocialChannel::Guild&&!IsInGuild(Character.CharacterId)){OutResult=EAetherSocialResult::NotGuildMember;return false;}

    const double* Last=LastChatAt.Find(Character.AccountId);
    if(Last&&Config.ChatCooldownSeconds>0.0f&&ServerTimeSeconds-*Last<Config.ChatCooldownSeconds){OutResult=EAetherSocialResult::ChatRateLimited;return false;}
    LastChatAt.Add(Character.AccountId,ServerTimeSeconds);

    OutMessage.Channel=Channel;
    OutMessage.SenderAccountId=Character.AccountId;
    OutMessage.SenderCharacterId=Character.CharacterId;
    OutMessage.TargetAccountId=TargetAccount;
    OutMessage.Message=Clean;
    OutMessage.ServerTimeSeconds=ServerTimeSeconds;
    OutResult=EAetherSocialResult::Accepted;
    return true;
}

void FAetherSocialService::ClearCharacterState(const FAetherCharacterId& CharacterId)
{
    FAetherSocialPartyId PartyId;
    if(IsInParty(CharacterId,&PartyId))
    {
        FAetherPartyState* Party=Parties.Find(PartyId);
        if(Party)
        {
            for(int32 I=0; I<Party->Members.Num(); ++I)
            {
                if(Party->Members[I].CharacterId == CharacterId)
                {
                    const bool bLeader = Party->Members[I].Role == EAetherPartyRole::Leader;
                    const FAetherAccountId RemovedAccount = Party->Members[I].AccountId;
                    Party->Members.RemoveAt(I);
                    PartyByAccount.Remove(RemovedAccount);
                    if(bLeader && Party->Members.Num() > 0)
                    {
                        Party->Members[0].Role = EAetherPartyRole::Leader;
                    }
                    break;
                }
            }
            if(Party->Members.Num() == 0)
            {
                PendingPartyInvites.Remove(PartyId);
                Parties.Remove(PartyId);
            }
        }
        PartyByCharacter.Remove(CharacterId);
    }
}

void FAetherSocialService::ClearAccountState(const FAetherAccountId& AccountId)
{
    LastChatAt.Remove(AccountId);
}
