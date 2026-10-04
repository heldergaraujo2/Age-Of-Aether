#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Aether2DEnvironmentVisualProfile.generated.h"

class UPaperSprite;

UENUM(BlueprintType)
enum class EAether2DEnvironmentAssetKind : uint8
{
    Architecture,
    Road,
    Vegetation,
    Rock,
    Water,
    Ruin,
    Dungeon,
    Prop,
    Interactive
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAether2DEnvironmentVisualProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    FName VisualProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    EAether2DEnvironmentAssetKind AssetKind = EAether2DEnvironmentAssetKind::Prop;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    FName FamilyID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    TSoftObjectPtr<UPaperSprite> Sprite;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    FVector2D VisualScale = FVector2D(1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    FVector SpriteWorldOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    int32 RenderLayer = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    bool bCastShadow = false;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Environment")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Environment")
    bool ValidateProfile(FString& OutError) const;
};
