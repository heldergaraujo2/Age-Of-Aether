#pragma once
#include "CoreMinimal.h"
#include "AetherClassPresentationTypes.generated.h"

USTRUCT(BlueprintType)
struct FAetherClassPresentationDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ClassID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString MeshAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString AnimationSetAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SelectionWidgetAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TransitionVFXAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TransitionSFXAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SkillPresentationAssetIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EquipmentSocketNames;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AllowedItemTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString FallbackPresentationID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ContentVersion = 1;

    bool IsValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherClassPresentationBinding
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString ClassID;
    UPROPERTY(BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(BlueprintReadOnly) FString MeshAssetID;
    UPROPERTY(BlueprintReadOnly) FString AnimationSetAssetID;
    UPROPERTY(BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(BlueprintReadOnly) FString SelectionWidgetAssetID;
    UPROPERTY(BlueprintReadOnly) FString TransitionVFXAssetID;
    UPROPERTY(BlueprintReadOnly) FString TransitionSFXAssetID;
    UPROPERTY(BlueprintReadOnly) bool bUsingFallback = false;
};

UENUM(BlueprintType)
enum class EAetherClassPresentationLoadState : uint8
{
    NotRequested,
    Loading,
    Ready,
    Fallback,
    Failed
};
