#pragma once
#include "CoreMinimal.h"
#include "AetherContentTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherContentType : uint8
{
    Generic,
    Item,
    Monster,
    NPC,
    Skill,
    Recipe,
    LootTable,
    Quest,
    Asset
};

USTRUCT(BlueprintType)
struct FAetherContentMetadata
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 SchemaVersion = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Category;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bDeprecated = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ContentVersion = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString LocalizationKey;

    bool IsValidBasic() const
    {
        return !DefinitionID.TrimStartAndEnd().IsEmpty() &&
            SchemaVersion > 0 && ContentVersion > 0;
    }
};

USTRUCT(BlueprintType)
struct FAetherContentDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherContentType Type = EAetherContentType::Generic;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherContentMetadata Metadata;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> References;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, double> NumericFields;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ProbabilityFields;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> NonNegativeFields;

    bool IsStructurallyValid(FString& OutError) const
    {
        if (!Metadata.IsValidBasic())
        {
            OutError = TEXT("Invalid definition metadata.");
            return false;
        }

        for (const TPair<FString, double>& Pair : NumericFields)
        {
            if (!FMath::IsFinite(Pair.Value))
            {
                OutError = FString::Printf(TEXT("Numeric field '%s' is not finite."), *Pair.Key);
                return false;
            }
        }

        for (const FString& Field : ProbabilityFields)
        {
            const double* Value = NumericFields.Find(Field);
            if (!Value || *Value < 0.0 || *Value > 1.0)
            {
                OutError = FString::Printf(TEXT("Probability field '%s' must be in [0,1]."), *Field);
                return false;
            }
        }

        for (const FString& Field : NonNegativeFields)
        {
            const double* Value = NumericFields.Find(Field);
            if (!Value || *Value < 0.0)
            {
                OutError = FString::Printf(TEXT("Non-negative field '%s' is missing or negative."), *Field);
                return false;
            }
        }

        return true;
    }
};
