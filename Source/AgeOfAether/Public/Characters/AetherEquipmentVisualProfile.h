#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/AetherItemDataTypes.h"
#include "AetherEquipmentVisualProfile.generated.h"
class UMaterialInterface; class USkeletalMesh; class UStaticMesh; class UPaperSprite;
UENUM(BlueprintType)
enum class EAetherEquipmentVisualType : uint8 { None, SkeletalMesh, StaticMesh, PaperSprite };
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherEquipmentVisualProfile : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FName VisualProfileID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FName ItemDefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") EAetherDataEquipmentSlot EquipmentSlot = EAetherDataEquipmentSlot::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") EAetherEquipmentVisualType VisualType = EAetherEquipmentVisualType::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") TSoftObjectPtr<UStaticMesh> StaticMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment|2D") TSoftObjectPtr<UPaperSprite> Sprite;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment|2D") FVector SpriteWorldOffset = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment|2D") FVector2D SpriteScale = FVector2D(1.0f, 1.0f);
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment|2D") int32 RenderLayer = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FName AttachSocket = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FTransform RelativeTransform = FTransform::Identity;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") TArray<TSoftObjectPtr<UMaterialInterface>> MaterialOverrides;
    UFUNCTION(BlueprintCallable, Category="Age of Aether|Equipment") bool ValidateProfile(FString& OutError) const;
};
