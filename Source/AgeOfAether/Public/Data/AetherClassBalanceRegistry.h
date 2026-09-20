#pragma once
#include "CoreMinimal.h"
#include "Data/AetherClassBalanceTypes.h"
class FAetherClassRegistry;
class FAetherClassBalanceRegistry
{
public:
 bool RegisterProfile(const FAetherBalanceProfileDefinition& Definition,FString& OutError);
 bool SetActiveProfile(const FString& ProfileID,FString& OutError);
 bool SetFallbackProfile(const FString& ProfileID,FString& OutError);
 bool Resolve(const FString& ProfileID,const FString& ClassID,const FString& EvolutionID,bool bPvP,FAetherClassBalanceModifiers& OutModifiers,FString& OutError) const;
 bool Validate(TArray<FAetherBalanceValidationIssue>& OutIssues,const FAetherClassRegistry* ClassRegistry=nullptr) const;
 void Reset();
 int32 NumProfiles() const;
 bool HasProfile(const FString& ProfileID) const;
 const FString& GetActiveProfileID() const{return ActiveProfileID;}
 const FString& GetFallbackProfileID() const{return FallbackProfileID;}
private:
 TMap<FString,FAetherBalanceProfileDefinition> Profiles;
 FString ActiveProfileID;
 FString FallbackProfileID=TEXT("production");
 static FString Normalize(const FString& In);
 static void AddIssue(TArray<FAetherBalanceValidationIssue>& Issues,const FString& Scope,const FString& Code,const FString& Message);
 const FAetherBalanceProfileDefinition* FindProfile(const FString& ProfileID) const;
};
