#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Aether2DCharacterVisualProfile.generated.h"

class UPaperFlipbook;

UENUM(BlueprintType)
enum class EAether2DCharacterVisualState : uint8
{
    Idle,
    Walk,
    Run,
    Attack,
    Hit,
    Death,
    Cast,
    Interaction
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAether2DCharacterVisualProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    FName VisualProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    TMap<EAether2DCharacterVisualState, TSoftObjectPtr<UPaperFlipbook>> Flipbooks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    FVector2D VisualScale = FVector2D(1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    FVector SpriteWorldOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    bool bUseAsPrimaryPresentation = false;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Visual")
    bool HasState(EAether2DCharacterVisualState State) const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Visual")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Visual")
    bool ValidateProfile(FString& OutError) const;
};
