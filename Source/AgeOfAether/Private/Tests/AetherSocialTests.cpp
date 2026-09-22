#include "Misc/AutomationTest.h"
#include "Social/AetherSocialService.h"

namespace
{
    FAetherCharacterRecord MakeCharacter(const FString& Name)
    {
        FAetherCharacterRecord C;
        C.CharacterId=FAetherCharacterId::NewId();
        C.AccountId=FAetherAccountId::NewId();
        C.Name=Name;
        C.Status=EAetherCharacterStatus::Active;
        C.Level=10;
        return C;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialFriendTest,"AgeOfAether.Social.Friends",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialFriendTest::RunTest(const FString&)
{
    FAetherSocialService S; FAetherAccountId A=FAetherAccountId::NewId(),B=FAetherAccountId::NewId(); FAetherSocialOperation O;
    TestTrue(TEXT("request"),S.SendFriendRequest(A,B,O)); TestTrue(TEXT("pending"),O.Friends.Num()>=1);
    TestFalse(TEXT("duplicate"),S.SendFriendRequest(A,B,O)); TestEqual(TEXT("pending result"),O.Result,EAetherSocialResult::RequestPending);
    TestTrue(TEXT("accept"),S.AcceptFriendRequest(B,A,O)); TestTrue(TEXT("accepted"),S.GetFriends(A,O.Friends)); TestEqual(TEXT("friend state"),O.Friends[0].State,EAetherFriendState::Accepted);
    TestTrue(TEXT("remove"),S.RemoveFriend(A,B,O));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialBlockTest,"AgeOfAether.Social.Block",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialBlockTest::RunTest(const FString&)
{
    FAetherSocialService S; FAetherAccountId A=FAetherAccountId::NewId(),B=FAetherAccountId::NewId(); FAetherSocialOperation O;
    TestTrue(TEXT("block"),S.BlockAccount(A,B,O)); TestFalse(TEXT("blocked request"),S.SendFriendRequest(B,A,O)); TestEqual(TEXT("blocked result"),O.Result,EAetherSocialResult::Blocked);
    TestTrue(TEXT("unblock"),S.UnblockAccount(A,B,O));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialPartyTest,"AgeOfAether.Social.Party",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialPartyTest::RunTest(const FString&)
{
    FAetherSocialService S; auto A=MakeCharacter(TEXT("A")); auto B=MakeCharacter(TEXT("B")); FAetherSocialOperation O;
    TestTrue(TEXT("create"),S.CreateParty(A,O)); const auto PartyId=O.Party.PartyId;
    TestTrue(TEXT("invite"),S.InviteToParty(A,B.AccountId,O));
    TestTrue(TEXT("accept"),S.AcceptPartyInvite(B,PartyId,O)); TestEqual(TEXT("members"),O.Party.Members.Num(),2);
    TestTrue(TEXT("kick"),S.KickFromParty(A,B.CharacterId,O)); TestEqual(TEXT("one left"),O.Party.Members.Num(),1);
    TestTrue(TEXT("leave"),S.LeaveParty(A,O));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialPartyCapacityTest,"AgeOfAether.Social.PartyCapacity",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialPartyCapacityTest::RunTest(const FString&)
{
    FAetherSocialService S; FAetherSocialConfig C=S.GetConfig(); C.MaxPartyMembers=2; TestTrue(TEXT("config"),S.SetConfig(C));
    auto A=MakeCharacter(TEXT("A")); auto B=MakeCharacter(TEXT("B")); auto Cc=MakeCharacter(TEXT("C")); FAetherSocialOperation O;
    TestTrue(TEXT("create"),S.CreateParty(A,O)); auto Id=O.Party.PartyId; TestTrue(TEXT("invite B"),S.InviteToParty(A,B.AccountId,O)); TestTrue(TEXT("accept B"),S.AcceptPartyInvite(B,Id,O));
    TestFalse(TEXT("invite full"),S.InviteToParty(A,Cc.AccountId,O)); TestEqual(TEXT("full"),O.Result,EAetherSocialResult::PartyFull);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialGuildTest,"AgeOfAether.Social.Guild",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialGuildTest::RunTest(const FString&)
{
    FAetherSocialService S; auto A=MakeCharacter(TEXT("A")); auto B=MakeCharacter(TEXT("B")); FAetherSocialOperation O;
    TestTrue(TEXT("create guild"),S.CreateGuild(A,TEXT("Knights"),O)); auto Id=O.Guild.GuildId;
    TestTrue(TEXT("invite"),S.InviteToGuild(A,B.AccountId,O)); TestTrue(TEXT("accept"),S.AcceptGuildInvite(B,Id,O));
    TestEqual(TEXT("two members"),O.Guild.Members.Num(),2);
    TestTrue(TEXT("promote"),S.SetGuildRole(A,B.CharacterId,EAetherGuildRole::Officer,O));
    TestTrue(TEXT("leave member"),S.LeaveGuild(B,O)); TestTrue(TEXT("leader leaves last"),S.LeaveGuild(A,O));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialSecurityTest,"AgeOfAether.Social.Security",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialSecurityTest::RunTest(const FString&)
{
    FAetherSocialService S; auto A=MakeCharacter(TEXT("A")); auto B=MakeCharacter(TEXT("B")); auto C=MakeCharacter(TEXT("C")); FAetherSocialOperation O;
    TestFalse(TEXT("self friend"),S.SendFriendRequest(A.AccountId,A.AccountId,O)); TestEqual(TEXT("self result"),O.Result,EAetherSocialResult::CannotTargetSelf);
    TestFalse(TEXT("nonleader kick"),S.KickFromParty(B,A.CharacterId,O)); TestEqual(TEXT("not member"),O.Result,EAetherSocialResult::NotPartyMember);
    FAetherChatMessage InvalidMessage;
    TestFalse(TEXT("invalid chat"),S.ValidateChat(A,EAetherSocialChannel::Local,FAetherAccountId(),TEXT(""),1.0,InvalidMessage,O.Result));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialChatTest,"AgeOfAether.Social.Chat",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialChatTest::RunTest(const FString&)
{
    FAetherSocialService S; auto A=MakeCharacter(TEXT("A")); FAetherChatMessage M; EAetherSocialResult R;
    TestTrue(TEXT("chat"),S.ValidateChat(A,EAetherSocialChannel::Local,FAetherAccountId(),TEXT(" hello "),10.0,M,R));
    TestEqual(TEXT("trimmed"),M.Message,FString(TEXT("hello")));
    TestFalse(TEXT("rate limit"),S.ValidateChat(A,EAetherSocialChannel::Local,FAetherAccountId(),TEXT("again"),10.2,M,R));
    TestEqual(TEXT("rate limited"),R,EAetherSocialResult::ChatRateLimited);
    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSocialCleanupTest,"AgeOfAether.Social.Cleanup",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherSocialCleanupTest::RunTest(const FString&)
{
    FAetherSocialService S;
    auto A=MakeCharacter(TEXT("A"));
    auto B=MakeCharacter(TEXT("B"));
    FAetherSocialOperation O;
    TestTrue(TEXT("party"),S.CreateParty(A,O));
    TestTrue(TEXT("guild"),S.CreateGuild(A,TEXT("PersistentGuild"),O));
    S.ClearCharacterState(A.CharacterId);

    FAetherPartyState Party;
    TestFalse(TEXT("party removed on logout"),S.GetPartyForCharacter(A.CharacterId,Party));

    FAetherGuildState Guild;
    TestTrue(TEXT("guild membership persists"),S.GetGuildForCharacter(A.CharacterId,Guild));
    TestEqual(TEXT("guild name"),Guild.Name,FString(TEXT("persistentguild")));
    return true;
}
