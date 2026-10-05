#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AetherDevelopmentWorldActor.generated.h"

class UMaterialInstanceDynamic;
class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UAnimSequence;
class USkeletalMesh;
class USkeletalMeshComponent;
class UStaticMesh;
class UStaticMeshComponent;

/**
 * Runtime-only, art-directed first-region diorama used by the development map.
 *
 * Built-in primitives keep a clean checkout playable; optional Fab imports add
 * denser authored vegetation, a forest-edge mansion, and ambient town walkers
 * after Scripts/import_fab_library_assets.py has run in the Unreal Editor.
 */
UCLASS()
class AGEOFAETHER_API AAetherDevelopmentWorldActor : public AActor
{
    GENERATED_BODY()

public:
    AAetherDevelopmentWorldActor();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<UStaticMeshComponent> Ground;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    FVector GroundScale = FVector(42.0f, 32.0f, 0.20f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    float GroundZ = -10.0f;

    UPROPERTY(EditAnywhere, Category = "Age of Aether|Ambient Characters")
    bool bEnableMageWalkerPlaceholder = true;

    UPROPERTY(EditAnywhere, Category = "Age of Aether|Ambient Characters")
    bool bPlaceIdleFabHorseAtStable = false;

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
    void LoadFabEnvironmentAssets();
    void BuildImportedVegetation();
    void BuildFabMansionLandmark();
    void BuildFabHorseAtStable();
    void BuildAmbientVillageNPCs();
    void UpdateAmbientVillageNPCs(float DeltaSeconds);
    FVector ChooseAmbientNPCDestination(const FVector& FromLocal);
    bool IsAmbientNPCPathClear(const FVector& StartLocal, const FVector& EndLocal) const;
    void AddFabFoliageInstance(
        UStaticMesh* Mesh,
        const FName& BatchName,
        const FVector& GroundLocation,
        float TargetHeight,
        float WidthScale,
        const FRotator& Rotation = FRotator::ZeroRotator);

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

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabTreeBroadleafMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabTreeSmallMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabPineMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabBushLargeMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabBushMediumMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabBushFlowerMesh;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMesh> RuntimeFabMansionMesh;

    UPROPERTY(Transient)
    TMap<FName, TObjectPtr<UInstancedStaticMeshComponent>> RuntimeFabInstancedComponents;

    UPROPERTY(Transient)
    TObjectPtr<UStaticMeshComponent> RuntimeFabMansionComponent;

    UPROPERTY(Transient)
    TObjectPtr<USkeletalMesh> RuntimeFabHorseMesh;

    UPROPERTY(Transient)
    TObjectPtr<UAnimSequence> RuntimeFabHorseIdleAnimation;

    UPROPERTY(Transient)
    TObjectPtr<USkeletalMeshComponent> RuntimeFabHorseComponent;

    UPROPERTY(Transient)
    TArray<TObjectPtr<USkeletalMeshComponent>> RuntimeAmbientNPCComponents;

    UPROPERTY(Transient)
    TObjectPtr<UAnimSequence> RuntimeAmbientNPCIdleAnimation;

    UPROPERTY(Transient)
    TObjectPtr<UAnimSequence> RuntimeAmbientNPCWalkAnimation;

    TArray<float> RuntimeAmbientNPCWalkSpeeds;
    TArray<float> RuntimeAmbientNPCGroundOffsets;
    TArray<float> RuntimeAmbientNPCPauseTimers;
    TArray<FVector> RuntimeAmbientNPCTargets;
    TArray<FVector> RuntimeAmbientWalkWaypoints;
    FRandomStream RuntimeAmbientRandomStream;

    bool bDioramaBuilt = false;
};
