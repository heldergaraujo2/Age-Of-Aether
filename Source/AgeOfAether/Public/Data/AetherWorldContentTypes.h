#pragma once
#include "CoreMinimal.h"
#include "AetherWorldContentTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherMapType : uint8 { OpenWorld, City, Dungeon, Instance, Event };

UENUM(BlueprintType)
enum class EAetherCombatRule : uint8 { Peaceful, PvP, PvPWithPenalty, NoCombat };

UENUM(BlueprintType)
enum class EAetherInteractionType : uint8 { Talk, Use, Harvest, Enter, Exit, Teleport, Quest, Forge, Craft, Container, Custom };

UENUM(BlueprintType)
enum class EAetherWorldRequirementType : uint8
{
    LevelAtLeast, LevelAtMost, ItemOwned, QuestCompleted, WorldTag, ContentUnlocked, SkillLevelAtLeast
};

UENUM(BlueprintType)
enum class EAetherWorldOutcomeType : uint8
{
    None, GrantQuest, CompleteQuest, GrantReward, StartEvent, Teleport, SetWorldTag, ClearWorldTag, StartRecipe
};

UENUM(BlueprintType)
enum class EAetherWorldPointType : uint8
{
    Spawn, Portal, Teleport, Checkpoint, Waypoint, Interaction, Generic
};

USTRUCT(BlueprintType)
struct FAetherWorldRequirement
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherWorldRequirementType Type = EAetherWorldRequirementType::LevelAtLeast;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ReferenceID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Value = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RequiredTag;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherWorldOutcome
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherWorldOutcomeType Type = EAetherWorldOutcomeType::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ReferenceID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetMapID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetPointID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Value;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherStreamingCellDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FIntPoint Coordinates = FIntPoint::ZeroValue;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D MinBounds = FVector2D::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D MaxBounds = FVector2D(1000.0, 1000.0);
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float LoadRadius = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredWorldTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AssetIDs;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherWorldZoneDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D MinBounds = FVector2D::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D MaxBounds = FVector2D(1000.0, 1000.0);
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherCombatRule CombatRule = EAetherCombatRule::Peaceful;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bSafeZone = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredWorldTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherWorldPointDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherWorldPointType Type = EAetherWorldPointType::Generic;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector Location = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FRotator Rotation = FRotator::ZeroRotator;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString StreamingCellID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherMapConnectionDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SourcePointID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetMapID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetPointID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherWorldRequirement> Requirements;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bBidirectional = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherWorldActorPlacementDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString WorldActorID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FTransform Transform = FTransform::Identity;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString StreamingCellID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SpawnGroupID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ContentIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherInteractionDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherInteractionType Type = EAetherInteractionType::Use;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString WorldActorID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString PointID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString StreamingCellID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float InteractionRadius = 150.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherWorldRequirement> Requirements;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherWorldOutcome> Outcomes;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherMapDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherMapType MapType = EAetherMapType::OpenWorld;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D MinBounds = FVector2D::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D MaxBounds = FVector2D(10000.0, 10000.0);
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RecommendedMinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RecommendedMaximumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 StreamingCellSize = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxStreamingCellCount = 4096;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherCombatRule DefaultCombatRule = EAetherCombatRule::Peaceful;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bSafeZone = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefaultSpawnPointID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredWorldTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AssetIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherStreamingCellDefinition> StreamingCells;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherWorldZoneDefinition> Zones;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherWorldPointDefinition> Points;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherMapConnectionDefinition> Connections;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherWorldActorPlacementDefinition> ActorPlacements;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherInteractionDefinition> Interactions;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> WorldContentIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SpawnGroupIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RecipeIDs;
    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherWorldContentValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
