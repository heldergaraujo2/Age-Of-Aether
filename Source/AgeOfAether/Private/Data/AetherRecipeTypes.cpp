#include "Data/AetherRecipeTypes.h"
#include "Math/UnrealMathUtility.h"

namespace
{
bool RequiredID(const FString& Value){return !Value.TrimStartAndEnd().IsEmpty();}
bool ValidFiniteNonNegative(double Value){return FMath::IsFinite(Value)&&Value>=0.0;}
bool ValidateOutputs(const TArray<FAetherRecipeOutput>& Outputs,FString& Error)
{
    for(const auto& Output:Outputs) if(!Output.IsStructurallyValid(Error)) return false;
    return true;
}
}

bool FAetherRecipeIngredient::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if(!RequiredID(ItemID)){OutError=TEXT("Ingredient ItemID is required.");return false;}
    if(Quantity<=0){OutError=TEXT("Ingredient Quantity must be positive.");return false;}
    return true;
}

bool FAetherRecipeOutput::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if(!RequiredID(ItemID)){OutError=TEXT("Output ItemID is required.");return false;}
    if(MinQuantity<=0||MaxQuantity<=0||MaxQuantity<MinQuantity){OutError=TEXT("Output quantity range is invalid.");return false;}
    return true;
}

bool FAetherRecipeUnlockCondition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if((Type==EAetherRecipeUnlockConditionType::QuestCompleted||
        Type==EAetherRecipeUnlockConditionType::ItemOwned||
        Type==EAetherRecipeUnlockConditionType::ContentUnlocked||
        Type==EAetherRecipeUnlockConditionType::SkillLevelAtLeast)&&!RequiredID(ReferenceID))
    {OutError=TEXT("Unlock condition ReferenceID is required.");return false;}
    if(Type==EAetherRecipeUnlockConditionType::WorldTag&&!RequiredID(RequiredTag))
    {OutError=TEXT("WorldTag unlock condition requires RequiredTag.");return false;}
    if((Type==EAetherRecipeUnlockConditionType::LevelAtLeast||
        Type==EAetherRecipeUnlockConditionType::SkillLevelAtLeast||
        Type==EAetherRecipeUnlockConditionType::QuestCompleted||
        Type==EAetherRecipeUnlockConditionType::ItemOwned)&&Value<0)
    {OutError=TEXT("Unlock condition Value cannot be negative.");return false;}
    return true;
}

bool FAetherRecipeDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if(!RequiredID(DefinitionID)||!RequiredID(DisplayName)){OutError=TEXT("Recipe ID and DisplayName are required.");return false;}
    if(!ValidFiniteNonNegative(SuccessChance)||SuccessChance>1.0){OutError=TEXT("SuccessChance must be in [0,1].");return false;}
    if(!ValidFiniteNonNegative(CooldownSeconds)){OutError=TEXT("CooldownSeconds must be finite and non-negative.");return false;}
    if(RequiredCharacterLevel<0||RequiredSkillLevel<0||DailyLimit<0||WeeklyLimit<0||RequiredCurrencyAmount<0){OutError=TEXT("Recipe requirement values cannot be negative.");return false;}
    if(Ingredients.Num()==0){OutError=TEXT("Recipe requires at least one ingredient.");return false;}
    if(Outputs.Num()==0){OutError=TEXT("Recipe requires at least one output.");return false;}
    if(!ValidateOutputs(Outputs,OutError)||!ValidateOutputs(FailureOutputs,OutError))return false;
    TSet<FString> IngredientIDs;
    for(const auto& Ingredient:Ingredients)
    {
        if(!Ingredient.IsStructurallyValid(OutError))return false;
        const FString ID=Ingredient.ItemID.TrimStartAndEnd();
        if(IngredientIDs.Contains(ID)){OutError=TEXT("Duplicate ingredient ItemID is not allowed.");return false;}
        IngredientIDs.Add(ID);
    }
    for(const auto& Condition:UnlockConditions)if(!Condition.IsStructurallyValid(OutError))return false;
    if(RequiredSkillLevel>0&&!RequiredID(RequiredSkillID)){OutError=TEXT("RequiredSkillID is required when RequiredSkillLevel is set.");return false;}
    if(RequiredCurrencyAmount>0&&!RequiredID(RequiredCurrencyID)){OutError=TEXT("RequiredCurrencyID is required when currency is required.");return false;}
    if(CatalystQuantity<0||ProtectionQuantity<0){OutError=TEXT("Catalyst/Protection quantities cannot be negative.");return false;}
    if(CatalystQuantity>0&&!RequiredID(CatalystItemID)){OutError=TEXT("CatalystItemID is required when CatalystQuantity is set.");return false;}
    if(ProtectionQuantity>0&&!RequiredID(ProtectionItemID)){OutError=TEXT("ProtectionItemID is required when ProtectionQuantity is set.");return false;}
    if(bGuaranteedResult&&GuaranteedAfterAttempts<=0){OutError=TEXT("GuaranteedAfterAttempts must be positive when guaranteed result is enabled.");return false;}
    if(!bGuaranteedResult&&GuaranteedAfterAttempts!=0){OutError=TEXT("GuaranteedAfterAttempts must be zero when guaranteed result is disabled.");return false;}
    if(FailureBehavior==EAetherRecipeFailureBehavior::ProduceFailureOutputs&&FailureOutputs.Num()==0){OutError=TEXT("ProduceFailureOutputs requires failure outputs.");return false;}
    return true;
}
