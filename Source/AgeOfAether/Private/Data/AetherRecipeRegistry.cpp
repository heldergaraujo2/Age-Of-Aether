#include "Data/AetherRecipeRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherSkillEffectRegistry.h"
#include "Data/AetherQuestDialogueEventRegistry.h"
#include "Data/AetherWorldActorRegistry.h"

namespace{FString N(const FString& ID){return ID.TrimStartAndEnd();}}

void FAetherRecipeRegistry::AddIssue(TArray<FAetherRecipeValidationIssue>& OutIssues,const FString& ID,const FString& Code,const FString& Message)const
{FAetherRecipeValidationIssue& I=OutIssues.AddDefaulted_GetRef();I.DefinitionID=ID;I.Code=Code;I.Message=Message;}

bool FAetherRecipeRegistry::RegisterRecipe(const FAetherRecipeDefinition& Definition,FString& OutError)
{
    OutError.Reset();
    FAetherRecipeDefinition Copy=Definition; Copy.DefinitionID=N(Copy.DefinitionID);
    if(!Copy.IsStructurallyValid(OutError))return false;
    if(Copy.DefinitionID.IsEmpty()||Definitions.Contains(Copy.DefinitionID)){OutError=FString::Printf(TEXT("Duplicate or empty Recipe DefinitionID '%s'."),*Copy.DefinitionID);return false;}
    Definitions.Add(Copy.DefinitionID,MoveTemp(Copy));return true;
}

bool FAetherRecipeRegistry::Resolve(const FString& DefinitionID,FAetherRecipeDefinition& OutDefinition)const
{const FAetherRecipeDefinition* Found=Definitions.Find(N(DefinitionID));if(!Found)return false;OutDefinition=*Found;return true;}

bool FAetherRecipeRegistry::Contains(const FString& DefinitionID)const{return Definitions.Contains(N(DefinitionID));}

bool FAetherRecipeRegistry::Validate(TArray<FAetherRecipeValidationIssue>& OutIssues,const FAetherContentRegistry* ContentRegistry,const FAetherAssetRegistry*,const FAetherItemRegistry* ItemRegistry,const FAetherSkillEffectRegistry* SkillRegistry,const FAetherQuestDialogueEventRegistry* QuestRegistry,const FAetherWorldActorRegistry* WorldActorRegistry)const
{
    OutIssues.Reset(); bool bValid=true;
    auto Add=[&](const FString&ID,const FString&Code,const FString&Msg){AddIssue(OutIssues,ID,Code,Msg);bValid=false;};
    for(const auto& Pair:Definitions)
    {
        const FAetherRecipeDefinition& D=Pair.Value; FString Error;
        if(!D.IsStructurallyValid(Error)){Add(D.DefinitionID,TEXT("InvalidDefinition"),Error);continue;}
        if(ContentRegistry&&!ContentRegistry->Contains(D.DefinitionID))Add(D.DefinitionID,TEXT("MissingContentDefinition"),FString::Printf(TEXT("Content definition '%s' is missing."),*D.DefinitionID));
        auto Item=[&](const FString&ID,const FString&Role){if(ItemRegistry&&!ItemRegistry->Contains(ID))Add(D.DefinitionID,TEXT("MissingItemReference"),FString::Printf(TEXT("%s item '%s' is missing.",*Role,*ID)));};
        for(const auto& Ingredient:D.Ingredients)Item(Ingredient.ItemID,TEXT("Ingredient"));
        for(const auto& Output:D.Outputs)Item(Output.ItemID,TEXT("Output"));
        for(const auto& Output:D.FailureOutputs)Item(Output.ItemID,TEXT("Failure output"));
        if(!D.CatalystItemID.IsEmpty())Item(D.CatalystItemID,TEXT("Catalyst"));
        if(!D.ProtectionItemID.IsEmpty())Item(D.ProtectionItemID,TEXT("Protection"));
        if(!D.RequiredSkillID.IsEmpty()&&SkillRegistry&&!SkillRegistry->Contains(D.RequiredSkillID))Add(D.DefinitionID,TEXT("MissingSkillReference"),FString::Printf(TEXT("Required skill '%s' is missing."),*D.RequiredSkillID));
        if(!D.RequiredStationID.IsEmpty()&&WorldActorRegistry&&!WorldActorRegistry->Contains(D.RequiredStationID))Add(D.DefinitionID,TEXT("MissingStationReference"),FString::Printf(TEXT("Required station '%s' is missing."),*D.RequiredStationID));
        for(const auto& Condition:D.UnlockConditions)
        {
            if(Condition.Type==EAetherRecipeUnlockConditionType::QuestCompleted&&QuestRegistry&&!QuestRegistry->Contains(Condition.ReferenceID))Add(D.DefinitionID,TEXT("MissingQuestReference"),FString::Printf(TEXT("Unlock quest '%s' is missing."),*Condition.ReferenceID));
            if(Condition.Type==EAetherRecipeUnlockConditionType::ItemOwned)Item(Condition.ReferenceID,TEXT("Unlock"));
            if(Condition.Type==EAetherRecipeUnlockConditionType::ContentUnlocked&&ContentRegistry&&!ContentRegistry->Contains(Condition.ReferenceID))Add(D.DefinitionID,TEXT("MissingContentReference"),FString::Printf(TEXT("Unlock content '%s' is missing."),*Condition.ReferenceID));
            if(Condition.Type==EAetherRecipeUnlockConditionType::SkillLevelAtLeast&&SkillRegistry&&!SkillRegistry->Contains(Condition.ReferenceID))Add(D.DefinitionID,TEXT("MissingSkillReference"),FString::Printf(TEXT("Unlock skill '%s' is missing."),*Condition.ReferenceID));
        }
    }
    return bValid&&OutIssues.Num()==0;
}

void FAetherRecipeRegistry::GetDefinitionIDs(TArray<FString>& OutIDs)const{Definitions.GetKeys(OutIDs);OutIDs.Sort();}
void FAetherRecipeRegistry::Reset(){Definitions.Reset();}
