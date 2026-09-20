#pragma once
#include "CoreMinimal.h"
#include "AetherClassTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherClassRole : uint8
{
    Damage, Tank, Healer, Support, Hybrid
};

USTRUCT(BlueprintType)
struct FAetherClassStatModifiers
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, double> Values;
};

USTRUCT(BlueprintType)
struct FAetherClassDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ClassID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString PathTitle;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherClassRole Role = EAetherClassRole::Hybrid;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 StartingLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherClassStatModifiers BaseStats;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AllowedItemTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> StartingItemIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> StartingSkillIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherClassEvolutionDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ClassID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EvolutionTitle;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Stage = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> PrerequisiteEvolutionIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredQuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredAchievementIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredContentIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherClassStatModifiers StatModifiers;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AllowedItemTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> GrantedSkillIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> GrantedEffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> VisualAssetIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherClassValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
