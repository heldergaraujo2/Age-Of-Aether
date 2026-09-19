#include "Social/AetherSocialSubsystem.h"

#define FORWARD(method) return SocialService.method
bool UAetherSocialSubsystem::SendFriendRequest(const FAetherAccountId& A,const FAetherAccountId& B,FAetherSocialOperation& O){return SocialService.SendFriendRequest(A,B,O);}
bool UAetherSocialSubsystem::AcceptFriendRequest(const FAetherAccountId& A,const FAetherAccountId& B,FAetherSocialOperation& O){return SocialService.AcceptFriendRequest(A,B,O);}
bool UAetherSocialSubsystem::RejectFriendRequest(const FAetherAccountId& A,const FAetherAccountId& B,FAetherSocialOperation& O){return SocialService.RejectFriendRequest(A,B,O);}
bool UAetherSocialSubsystem::RemoveFriend(const FAetherAccountId& A,const FAetherAccountId& B,FAetherSocialOperation& O){return SocialService.RemoveFriend(A,B,O);}
bool UAetherSocialSubsystem::BlockAccount(const FAetherAccountId& A,const FAetherAccountId& B,FAetherSocialOperation& O){return SocialService.BlockAccount(A,B,O);}
bool UAetherSocialSubsystem::UnblockAccount(const FAetherAccountId& A,const FAetherAccountId& B,FAetherSocialOperation& O){return SocialService.UnblockAccount(A,B,O);}
bool UAetherSocialSubsystem::GetFriends(const FAetherAccountId& A,TArray<FAetherSocialRelation>& O)const{return SocialService.GetFriends(A,O);}
bool UAetherSocialSubsystem::CreateParty(const FAetherCharacterRecord& C,FAetherSocialOperation& O){return SocialService.CreateParty(C,O);}
bool UAetherSocialSubsystem::InviteToParty(const FAetherCharacterRecord& C,const FAetherAccountId& T,FAetherSocialOperation& O){return SocialService.InviteToParty(C,T,O);}
bool UAetherSocialSubsystem::AcceptPartyInvite(const FAetherCharacterRecord& C,const FAetherSocialPartyId& I,FAetherSocialOperation& O){return SocialService.AcceptPartyInvite(C,I,O);}
bool UAetherSocialSubsystem::LeaveParty(const FAetherCharacterRecord& C,FAetherSocialOperation& O){return SocialService.LeaveParty(C,O);}
bool UAetherSocialSubsystem::KickFromParty(const FAetherCharacterRecord& C,const FAetherCharacterId& T,FAetherSocialOperation& O){return SocialService.KickFromParty(C,T,O);}
bool UAetherSocialSubsystem::GetPartyForCharacter(const FAetherCharacterId& C,FAetherPartyState& O)const{return SocialService.GetPartyForCharacter(C,O);}
bool UAetherSocialSubsystem::CreateGuild(const FAetherCharacterRecord& C,const FString& N,FAetherSocialOperation& O){return SocialService.CreateGuild(C,N,O);}
bool UAetherSocialSubsystem::InviteToGuild(const FAetherCharacterRecord& C,const FAetherAccountId& T,FAetherSocialOperation& O){return SocialService.InviteToGuild(C,T,O);}
bool UAetherSocialSubsystem::AcceptGuildInvite(const FAetherCharacterRecord& C,const FAetherGuildId& I,FAetherSocialOperation& O){return SocialService.AcceptGuildInvite(C,I,O);}
bool UAetherSocialSubsystem::LeaveGuild(const FAetherCharacterRecord& C,FAetherSocialOperation& O){return SocialService.LeaveGuild(C,O);}
bool UAetherSocialSubsystem::SetGuildRole(const FAetherCharacterRecord& C,const FAetherCharacterId& T,EAetherGuildRole R,FAetherSocialOperation& O){return SocialService.SetGuildRole(C,T,R,O);}
bool UAetherSocialSubsystem::GetGuildForCharacter(const FAetherCharacterId& C,FAetherGuildState& O)const{return SocialService.GetGuildForCharacter(C,O);}
bool UAetherSocialSubsystem::ValidateChat(const FAetherCharacterRecord& C,EAetherSocialChannel Ch,const FAetherAccountId& T,const FString& M,double S,FAetherChatMessage& O,EAetherSocialResult& R){return SocialService.ValidateChat(C,Ch,T,M,S,O,R);}
void UAetherSocialSubsystem::ClearCharacterState(const FAetherCharacterId& CharacterId){SocialService.ClearCharacterState(CharacterId);}
void UAetherSocialSubsystem::ClearAccountState(const FAetherAccountId& AccountId){SocialService.ClearAccountState(AccountId);}

#undef FORWARD
