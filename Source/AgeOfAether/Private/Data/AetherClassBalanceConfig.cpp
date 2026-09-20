#include "Data/AetherClassBalanceConfig.h"
#include "Misc/FileHelper.h"
namespace
{
bool Number(const TArray<FString>& P,int32 I,double&O){if(!P.IsValidIndex(I)||!LexTryParseString(O,*P[I]))return false;return FMath::IsFinite(O);}
bool Header(const FString& L){return L.StartsWith(TEXT("#"))||L.TrimStartAndEnd().IsEmpty();}
bool ParseMod(const TArray<FString>& P,int32 S,FAetherClassBalanceModifiers&O)
{
 double* V[]={&O.Damage,&O.CriticalDamage,&O.Defense,&O.Healing,&O.Shield,&O.AreaDamage,&O.CrowdControl,&O.CrowdControlResistance,&O.MovementSpeed,&O.AttackSpeed,&O.CastSpeed,&O.Threat,&O.LifeSteal,&O.Regeneration,&O.BuffEffectiveness,&O.DebuffEffectiveness,&O.OutgoingDamage,&O.IncomingDamage,&O.ResourceCost,&O.ResourceRecovery};
 for(int32 I=0;I<20;++I)if(!Number(P,S+I,*V[I]))return false;return true;
}
}
bool FAetherClassBalanceConfigLoader::Parse(const FString& Text,FAetherBalanceConfig& O,FString&E)
{
 O=FAetherBalanceConfig();E.Reset();TArray<FString> Lines;Text.ParseIntoArrayLines(Lines);
 TMap<FString,int32> ProfileIndex;
 for(int32 N=0;N<Lines.Num();++N){FString L=Lines[N].TrimStartAndEnd();if(Header(L))continue;TArray<FString>P;L.ParseIntoArray(P,TEXT("|"),false);if(P.Num()==0)continue;
  if(P[0].Equals(TEXT("config"),ESearchCase::IgnoreCase)){if(P.Num()<4||!LexTryParseString(O.ConfigVersion,*P[1])||O.ConfigVersion<=0){E=FString::Printf(TEXT("Invalid config line %d."),N+1);return false;}O.ActiveProfileID=P[2].TrimStartAndEnd().ToLower();O.FallbackProfileID=P[3].TrimStartAndEnd().ToLower();}
  else if(P[0].Equals(TEXT("profile"),ESearchCase::IgnoreCase)){if(P.Num()<3){E=FString::Printf(TEXT("Invalid profile line %d."),N+1);return false;}FAetherBalanceProfileDefinition D;D.ProfileID=P[1].TrimStartAndEnd().ToLower();if(!LexTryParseString(D.Version,*P[2])||D.Version<=0||D.ProfileID.IsEmpty()||ProfileIndex.Contains(D.ProfileID)){E=FString::Printf(TEXT("Invalid or duplicate profile at line %d."),N+1);return false;}ProfileIndex.Add(D.ProfileID,O.Profiles.Add(D));}
  else if(P[0].Equals(TEXT("balance"),ESearchCase::IgnoreCase)){if(P.Num()!=45){E=FString::Printf(TEXT("Invalid balance line %d."),N+1);return false;}const FString PID=P[1].TrimStartAndEnd().ToLower();const int32* Index=ProfileIndex.Find(PID);if(!Index){E=FString::Printf(TEXT("Balance references unknown profile at line %d."),N+1);return false;}FAetherClassBalanceDefinition D;D.DefinitionID=P[2].TrimStartAndEnd().ToLower();D.ClassID=P[3].TrimStartAndEnd().ToLower();D.EvolutionID=P[4].TrimStartAndEnd().ToLower();if(!ParseMod(P,5,D.PvE)||!ParseMod(P,25,D.PvP)){E=FString::Printf(TEXT("Invalid balance numbers at line %d."),N+1);return false;}O.Profiles[*Index].Definitions.Add(D);}
  else {E=FString::Printf(TEXT("Unknown directive at line %d."),N+1);return false;}
 }
 if(O.ConfigVersion<=0||O.ActiveProfileID.IsEmpty()||O.FallbackProfileID.IsEmpty()||O.Profiles.Num()==0){E=TEXT("Config requires version, active/fallback profiles and at least one profile.");return false;}
 return true;
}
bool FAetherClassBalanceConfigLoader::LoadFile(const FString& FilePath,FAetherBalanceConfig& O,FString&E)
{
 FString Text;
 if(!FFileHelper::LoadFileToString(Text,*FilePath)){E=FString::Printf(TEXT("Unable to load balance configuration: %s"),*FilePath);return false;}
 return Parse(Text,O,E);
}
FString FAetherClassBalanceConfigLoader::Serialize(const FAetherBalanceConfig&C)
{
 FString O=FString::Printf(TEXT("# AGE OF AETHER CLASS BALANCE v%d\nconfig|%d|%s|%s\n"),C.ConfigVersion,C.ConfigVersion,*C.ActiveProfileID,*C.FallbackProfileID);
 for(const auto&P:C.Profiles){O+=FString::Printf(TEXT("profile|%s|%d\n"),*P.ProfileID,P.Version);for(const auto&D:P.Definitions){O+=FString::Printf(TEXT("balance|%s|%s|%s|%s"),*P.ProfileID,*D.DefinitionID,*D.ClassID,*D.EvolutionID);O+=TEXT("|");const double V1[]={D.PvE.Damage,D.PvE.CriticalDamage,D.PvE.Defense,D.PvE.Healing,D.PvE.Shield,D.PvE.AreaDamage,D.PvE.CrowdControl,D.PvE.CrowdControlResistance,D.PvE.MovementSpeed,D.PvE.AttackSpeed,D.PvE.CastSpeed,D.PvE.Threat,D.PvE.LifeSteal,D.PvE.Regeneration,D.PvE.BuffEffectiveness,D.PvE.DebuffEffectiveness,D.PvE.OutgoingDamage,D.PvE.IncomingDamage,D.PvE.ResourceCost,D.PvE.ResourceRecovery};for(int32 I=0;I<20;++I){if(I>0)O+=TEXT("|");O+=FString::SanitizeFloat(V1[I]);}O+=TEXT("|");const double V2[]={D.PvP.Damage,D.PvP.CriticalDamage,D.PvP.Defense,D.PvP.Healing,D.PvP.Shield,D.PvP.AreaDamage,D.PvP.CrowdControl,D.PvP.CrowdControlResistance,D.PvP.MovementSpeed,D.PvP.AttackSpeed,D.PvP.CastSpeed,D.PvP.Threat,D.PvP.LifeSteal,D.PvP.Regeneration,D.PvP.BuffEffectiveness,D.PvP.DebuffEffectiveness,D.PvP.OutgoingDamage,D.PvP.IncomingDamage,D.PvP.ResourceCost,D.PvP.ResourceRecovery};for(int32 I=0;I<20;++I){if(I>0)O+=TEXT("|");O+=FString::SanitizeFloat(V2[I]);}O+=TEXT("\n");}}return O;
}
bool FAetherClassBalanceConfigLoader::BuildRegistry(const FAetherBalanceConfig&C,FAetherClassBalanceRegistry&R,FString&E)
{
 R.Reset();if(C.ConfigVersion<=0){E=TEXT("Invalid config version.");return false;}for(const auto&P:C.Profiles)if(!R.RegisterProfile(P,E))return false;if(!R.SetFallbackProfile(C.FallbackProfileID,E)||!R.SetActiveProfile(C.ActiveProfileID,E)){return false;}return true;
}
