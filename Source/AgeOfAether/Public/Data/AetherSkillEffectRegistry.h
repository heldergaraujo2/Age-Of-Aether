#pragma once
#include "CoreMinimal.h"
#include "Data/AetherSkillEffectTypes.h"
class FAetherContentRegistry;
class FAetherAssetRegistry;
class FAetherSkillEffectRegistry
{
public:
 bool RegisterSkill(const FAetherDataSkillDefinition&, FString&);
 bool RegisterEffect(const FAetherEffectDefinition&, FString&);
 bool RegisterStatus(const FAetherStatusDefinition&, FString&);
 bool ResolveSkill(const FString&, FAetherDataSkillDefinition&) const;
 bool ResolveEffect(const FString&, FAetherEffectDefinition&) const;
 bool ResolveStatus(const FString&, FAetherStatusDefinition&) const;
 bool Contains(const FString&) const;
 bool Validate(TArray<FAetherSkillEffectValidationIssue>& Issues, const FAetherContentRegistry* ContentRegistry = nullptr, const FAetherAssetRegistry* AssetRegistry = nullptr) const;
 void GetDefinitionIDs(TArray<FString>&) const;
 int32 NumSkills() const { return Skills.Num(); }
 int32 NumEffects() const { return Effects.Num(); }
 int32 NumStatuses() const { return Statuses.Num(); }
 void Reset();
private:
 TMap<FString,FAetherDataSkillDefinition> Skills;
 TMap<FString,FAetherEffectDefinition> Effects;
 TMap<FString,FAetherStatusDefinition> Statuses;
 bool IsIDAvailable(const FString&) const;
 void AddIssue(TArray<FAetherSkillEffectValidationIssue>&, const FString&, const FString&, const FString&) const;
};
