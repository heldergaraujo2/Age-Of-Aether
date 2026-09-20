#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/AetherItemTypes.h"
#include "AetherEquipmentVisualProfile.generated.h"
class UMaterialInterface; class USkeletalMesh; class UStaticMesh;
UENUM(BlueprintType)
enum class EAetherEquipmentVisualType : uint8 { None, SkeletalMesh, StaticMesh };
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherEquipmentVisualProfile : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FName VisualProfileID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FName ItemDefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") EAetherEquipmentSlot EquipmentSlot = EAetherEquipmentSlot::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") EAetherEquipmentVisualType VisualType = EAetherEquipmentVisualType::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") TSoftObjectPtr<UStaticMesh> StaticMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FName AttachSocket = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") FTransform RelativeTransform = FTransform::Identity;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Equipment") TArray<TSoftObjectPtr<UMaterialInterface>> MaterialOverrides;
    UFUNCTION(BlueprintCallable, Category="Age of Aether|Equipment") bool ValidateProfile(FString& OutError) const;
};