#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Characters/Aether2DCharacterVisualProfile.h"
#include "Aether2DLivingVisualProfile.generated.h"

class UPaperFlipbook;

UENUM(BlueprintType)
enum class EAether2DLivingVisualKind : uint8
{
    Creature,
    NPC,
    Boss
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAether2DLivingVisualProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    FName VisualProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    EAether2DLivingVisualKind VisualKind = EAether2DLivingVisualKind::Creature;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    FName FamilyID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    TMap<EAether2DCharacterVisualState, TSoftObjectPtr<UPaperFlipbook>> Flipbooks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    FVector2D VisualScale = FVector2D(1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    FVector SpriteWorldOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    bool bUseAsPrimaryPresentation = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    bool bDriveStateFromMovement = true;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Living Visual")
    bool HasState(EAether2DCharacterVisualState State) const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Living Visual")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Living Visual")
    bool ValidateProfile(FString& OutError) const;
};
