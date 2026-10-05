#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AetherDevelopmentWorldActor.generated.h"

class UMaterialInstanceDynamic;
class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UStaticMesh;
class UStaticMeshComponent;

/**
 * Runtime-only, art-directed first-region diorama used by the development map.
 *
 * The scene is assembled from Unreal's built-in meshes so a clean project can
 * show the complete playable composition without fabricated .uasset/.umap
 * files. Production meshes/materials can replace these pieces incrementally.
 */
UCLASS()
class AGEOFAETHER_API AAetherDevelopmentWorldActor : public AActor
{
    GENERATED_BODY()

public:
    AAetherDevelopmentWorldActor();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<UStaticMeshComponent> Ground;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    FVector GroundScale = FVector(42.0f, 32.0f, 0.20f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    float GroundZ = -10.0f;

private:
    void ConfigureGround();
    void BuildFirstRegionDiorama();
    void BuildTerrain();
    void BuildGroundCover();
    void BuildRiverAndRoads();
    void BuildBridge();
    void BuildVillage();
    void BuildCastle();
    void BuildFarms();
    void BuildForest();
    void BuildLandmarks();

    UStaticMeshComponent* AddPrimitive(
        UStaticMesh* Mesh,
        const FName& Name,
        const FVector& Location,
        const FVector& Scale,
        const FLinearColor& Color,
        bool bBlockMovement = false,
        const FRotator& Rotation = FRotator::ZeroRotator);
    void AddInstancedPrimitive(
        UStaticMesh* Mesh,
        const FName& Name,
        const FVector& Location,
        const FVector& Scale,
        const FLinearColor& Color,
        bool bBlockMovement = false,
        const FRotator& Rotation = FRotator::ZeroRotator,
        bool bCastShadow = true);

    UMaterialInstanceDynamic* CreateColorMaterial(const FLinearColor& Color);
    void AddRibbon(
        const TArray<FVector>& Points,
        float Width,
        float Z,
        float Thickness,
        const FLinearColor& Color,
        const FName& NamePrefix,
        bool bRoundJoints = true);
    void BuildHouse(
        const FVector& Location,
        float Scale,
        const FLinearColor& RoofColor,
        const FName& NamePrefix,
        float Yaw = 0.0f,
        const FLinearColor& WallColor = FLinearColor(0.86f, 0.78f, 0.60f, 1.0f));
    void BuildTower(
        const FVector& Location,
        float Height,
        float Radius,
        const FLinearColor& RoofColor,
        const FName& NamePrefix);
    void BuildTree(const FVector& Location, float Scale, int32 Variant, const FName& NamePrefix);
    void BuildRockCluster(const FVector& Location, float Scale, const FName& NamePrefix);
    void BuildFarmPlot(
        const FVector& Center,
        float Width,
        float Depth,
        float Yaw,
        const FLinearColor& SoilColor,
        const FLinearColor& CropColor,
        const FName& NamePrefix);
    void BuildFence(
        const FVector& Start,
        const FVector& End,
        float Height,
        const FName& NamePrefix,
        const FLinearColor& Color);
    void BuildMarketStall(
        const FVector& Location,
        const FLinearColor& CanopyColor,
        const FName& NamePrefix);
    void BuildLantern(const FVector& Location, const FName& NamePrefix);

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInterface> RuntimeBaseMaterial;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeCubeMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeCylinderMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeSphereMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeConeMesh;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UStaticMeshComponent>> RuntimeVisualComponents;

    UPROPERTY(Transient)
    TMap<uint32, TObjectPtr<UMaterialInstanceDynamic>> RuntimeMaterials;

    UPROPERTY(Transient)
    TMap<uint32, TObjectPtr<UInstancedStaticMeshComponent>> RuntimeInstancedComponents;

    bool bDioramaBuilt = false;
};
