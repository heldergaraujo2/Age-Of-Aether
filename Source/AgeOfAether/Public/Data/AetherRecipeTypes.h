#pragma once
#include "CoreMinimal.h"
#include "AetherRecipeTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherRecipeFailureBehavior : uint8
{
    PreserveInputs,
    ConsumeInputs,
    ProduceFailureOutputs
};

UENUM(BlueprintType)
enum class EAetherRecipeUnlockConditionType : uint8
{
    LevelAtLeast,
    QuestCompleted,
    ItemOwned,
    WorldTag,
    ContentUnlocked,
    SkillLevelAtLeast
};

USTRUCT(BlueprintType)
struct FAetherRecipeIngredient
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Quantity = 1;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherRecipeOutput
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinQuantity = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxQuantity = 1;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherRecipeUnlockCondition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherRecipeUnlockConditionType Type = EAetherRecipeUnlockConditionType::LevelAtLeast;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ReferenceID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Value = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RequiredTag;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherRecipeDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Category;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherRecipeIngredient> Ingredients;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherRecipeOutput> Outputs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherRecipeOutput> FailureOutputs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double SuccessChance = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherRecipeFailureBehavior FailureBehavior = EAetherRecipeFailureBehavior::PreserveInputs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredCharacterLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RequiredSkillID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredSkillLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RequiredStationID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RequiredCurrencyID = TEXT("Currency.Gold");
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 RequiredCurrencyAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CooldownSeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 DailyLimit = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 WeeklyLimit = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherRecipeUnlockCondition> UnlockConditions;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString CatalystItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 CatalystQuantity = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ProtectionItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ProtectionQuantity = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bGuaranteedResult = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 GuaranteedAfterAttempts = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherRecipeValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
