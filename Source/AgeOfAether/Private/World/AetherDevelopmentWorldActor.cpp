#include "World/AetherDevelopmentWorldActor.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "CollisionQueryParams.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/RotationMatrix.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
    const FLinearColor GrassColor(0.30f, 0.49f, 0.20f, 1.0f);
    const FLinearColor LightGrassColor(0.40f, 0.60f, 0.26f, 1.0f);
    const FLinearColor DeepGrassColor(0.20f, 0.38f, 0.16f, 1.0f);
    const FLinearColor StoneColor(0.55f, 0.55f, 0.49f, 1.0f);
    const FLinearColor WarmStoneColor(0.72f, 0.65f, 0.48f, 1.0f);
    const FLinearColor TimberColor(0.25f, 0.12f, 0.07f, 1.0f);
    const FLinearColor WaterColor(0.12f, 0.58f, 0.76f, 1.0f);
    const FLinearColor DeepWaterColor(0.08f, 0.36f, 0.58f, 1.0f);
    const FLinearColor BlueRoofColor(0.10f, 0.28f, 0.66f, 1.0f);

    FVector OffsetAlongYaw(const FVector& Origin, float Distance, float YawDegrees)
    {
        const float YawRadians = FMath::DegreesToRadians(YawDegrees);
        return Origin + FVector(FMath::Cos(YawRadians) * Distance, FMath::Sin(YawRadians) * Distance, 0.0f);
    }

    float GetAmbientNPCWalkPlayRate(float WalkSpeed)
    {
        constexpr float ReferenceWalkSpeed = 100.0f;
        constexpr float BasePlayRate = 1.25f;
        return FMath::Clamp((WalkSpeed / ReferenceWalkSpeed) * BasePlayRate, 0.75f, 1.55f);
    }

    void PlayLoopingSkeletalAnimation(
        USkeletalMeshComponent* SkeletalMesh,
        UAnimSequence* Animation,
        float PlayRate = 1.0f)
    {
        if (!SkeletalMesh || !Animation)
        {
            return;
        }

        SkeletalMesh->PlayAnimation(Animation, true);
        if (UAnimSingleNodeInstance* SingleNodeInstance = SkeletalMesh->GetSingleNodeInstance())
        {
            SingleNodeInstance->SetPlayRate(PlayRate);
        }
    }
}

AAetherDevelopmentWorldActor::AAetherDevelopmentWorldActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    Root->SetMobility(EComponentMobility::Movable);
    SetRootComponent(Root);

    Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
    Ground->SetupAttachment(Root);
    Ground->SetCollisionProfileName(TEXT("BlockAll"));
    Ground->SetMobility(EComponentMobility::Movable);
    Ground->SetCastShadow(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    if (CubeMesh.Succeeded())
    {
        RuntimeCubeMesh = CubeMesh.Object;
        Ground->SetStaticMesh(CubeMesh.Object);
    }
    if (CylinderMesh.Succeeded())
    {
        RuntimeCylinderMesh = CylinderMesh.Object;
    }
    if (SphereMesh.Succeeded())
    {
        RuntimeSphereMesh = SphereMesh.Object;
    }
    if (ConeMesh.Succeeded())
    {
        RuntimeConeMesh = ConeMesh.Object;
    }
    if (BaseMaterial.Succeeded())
    {
        RuntimeBaseMaterial = BaseMaterial.Object;
    }

    ConfigureGround();
}

void AAetherDevelopmentWorldActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    ConfigureGround();
}

void AAetherDevelopmentWorldActor::BeginPlay()
{
    Super::BeginPlay();

    if (GetNetMode() != NM_DedicatedServer)
    {
        BuildFirstRegionDiorama();
    }
}

void AAetherDevelopmentWorldActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateAmbientVillageNPCs(DeltaSeconds);
}

void AAetherDevelopmentWorldActor::ConfigureGround()
{
    if (!Ground)
    {
        return;
    }

    Ground->SetRelativeScale3D(GroundScale);
    Ground->SetRelativeLocation(FVector(0.0f, 0.0f, GroundZ));
}

void AAetherDevelopmentWorldActor::LoadEnvironmentAssets()
{
    RuntimeGrassGroundMaterial = LoadObject<UMaterialInterface>(
        nullptr, TEXT("/Game/Aether/Environment/Ground/Materials/M_GrassGround.M_GrassGround"));
    RuntimeFabTreeBroadleafMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/TreesBush/SM_Fab_Tree2.SM_Fab_Tree2"));
    RuntimeFabTreeSmallMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/TreesBush/SM_Fab_TreeSmall.SM_Fab_TreeSmall"));
    RuntimeFabPineMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/TreesBush/SM_Fab_Pine2.SM_Fab_Pine2"));
    RuntimeFabBushLargeMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/TreesBush/SM_Fab_BushBig.SM_Fab_BushBig"));
    RuntimeFabBushMediumMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/TreesBush/SM_Fab_BushMedium.SM_Fab_BushMedium"));
    RuntimeFabBushFlowerMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/TreesBush/SM_Fab_BushFlowers.SM_Fab_BushFlowers"));
    RuntimeFabMansionMesh = LoadObject<UStaticMesh>(
        nullptr, TEXT("/Game/Aether/Environment/Fab/Mansion/SM_Fab_HauntedMansion.SM_Fab_HauntedMansion"));
    RuntimeFabHorseMesh = LoadObject<USkeletalMesh>(
        nullptr, TEXT("/Game/Aether/Characters/FabHorse/SK_Fab_UnicornHorse.SK_Fab_UnicornHorse"));
    RuntimeFabHorseIdleAnimation = LoadObject<UAnimSequence>(
        nullptr, TEXT("/Game/Aether/Characters/FabHorse/Animations/A_Fab_UnicornHorse_Idle.A_Fab_UnicornHorse_Idle"));

    RuntimeCC0BroadleafTreeMeshes.Reset();
    RuntimeCC0PineTreeMeshes.Reset();
    RuntimeCC0BushMeshes.Reset();
    RuntimeCC0GroundCoverMeshes.Reset();
    RuntimeCC0WildflowerMeshes.Reset();

    const auto LoadCC0Meshes = [](TArray<TObjectPtr<UStaticMesh>>& Destination, const TCHAR* const* ObjectPaths, int32 PathCount)
    {
        for (int32 Index = 0; Index < PathCount; ++Index)
        {
            if (UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, ObjectPaths[Index]))
            {
                Destination.Add(Mesh);
            }
        }
    };

    const TCHAR* CC0BroadleafTreePaths[] = {
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Tree_1_A_Color1.SM_CC0_KayKit_Tree_1_A_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Tree_1_C_Color1.SM_CC0_KayKit_Tree_1_C_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Tree_3_A_Color1.SM_CC0_KayKit_Tree_3_A_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Tree_3_C_Color1.SM_CC0_KayKit_Tree_3_C_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Tree_4_A_Color1.SM_CC0_KayKit_Tree_4_A_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Tree_4_C_Color1.SM_CC0_KayKit_Tree_4_C_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_detailed_jungle.SM_CC0_Kenney_tree_detailed_jungle"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_fat_jungle.SM_CC0_Kenney_tree_fat_jungle"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_oak_jungle.SM_CC0_Kenney_tree_oak_jungle"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_plateau_jungle.SM_CC0_Kenney_tree_plateau_jungle"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_blocks_jungle.SM_CC0_Kenney_tree_blocks_jungle"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_tall_jungle.SM_CC0_Kenney_tree_tall_jungle")
    };
    const TCHAR* CC0PineTreePaths[] = {
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineSmallA.SM_CC0_Kenney_tree_pineSmallA"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineSmallB.SM_CC0_Kenney_tree_pineSmallB"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineTallA.SM_CC0_Kenney_tree_pineTallA"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineTallB.SM_CC0_Kenney_tree_pineTallB"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineRoundA.SM_CC0_Kenney_tree_pineRoundA"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineRoundC.SM_CC0_Kenney_tree_pineRoundC"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_tree_pineDefaultA.SM_CC0_Kenney_tree_pineDefaultA")
    };
    const TCHAR* CC0BushPaths[] = {
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Bush_1_E_Color1.SM_CC0_KayKit_Bush_1_E_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Bush_3_B_Color1.SM_CC0_KayKit_Bush_3_B_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_plant_bush.SM_CC0_Kenney_plant_bush"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_plant_bushLarge.SM_CC0_Kenney_plant_bushLarge"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_plant_bushDetailed.SM_CC0_Kenney_plant_bushDetailed")
    };
    const TCHAR* CC0GroundCoverPaths[] = {
        TEXT("/Game/Aether/Environment/CC0Forest/KayKit/SM_CC0_KayKit_Grass_2_D_Color1.SM_CC0_KayKit_Grass_2_D_Color1"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_grass_leafsLarge.SM_CC0_Kenney_grass_leafsLarge"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_grass_leafs.SM_CC0_Kenney_grass_leafs"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_grass_large.SM_CC0_Kenney_grass_large")
    };
    const TCHAR* CC0WildflowerPaths[] = {
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_flower_purpleA.SM_CC0_Kenney_flower_purpleA"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_flower_redA.SM_CC0_Kenney_flower_redA"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_flower_yellowA.SM_CC0_Kenney_flower_yellowA"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_flower_purpleC.SM_CC0_Kenney_flower_purpleC"),
        TEXT("/Game/Aether/Environment/CC0Forest/Kenney/SM_CC0_Kenney_flower_yellowC.SM_CC0_Kenney_flower_yellowC")
    };
    LoadCC0Meshes(RuntimeCC0BroadleafTreeMeshes, CC0BroadleafTreePaths, UE_ARRAY_COUNT(CC0BroadleafTreePaths));
    LoadCC0Meshes(RuntimeCC0PineTreeMeshes, CC0PineTreePaths, UE_ARRAY_COUNT(CC0PineTreePaths));
    LoadCC0Meshes(RuntimeCC0BushMeshes, CC0BushPaths, UE_ARRAY_COUNT(CC0BushPaths));
    LoadCC0Meshes(RuntimeCC0GroundCoverMeshes, CC0GroundCoverPaths, UE_ARRAY_COUNT(CC0GroundCoverPaths));
    LoadCC0Meshes(RuntimeCC0WildflowerMeshes, CC0WildflowerPaths, UE_ARRAY_COUNT(CC0WildflowerPaths));

    if (RuntimeCC0BroadleafTreeMeshes.Num() > 0 || RuntimeCC0PineTreeMeshes.Num() > 0
        || RuntimeCC0BushMeshes.Num() > 0 || RuntimeCC0GroundCoverMeshes.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("CC0 environment meshes loaded: broadleaf=%d pines=%d shrubs=%d grass=%d flowers=%d."),
            RuntimeCC0BroadleafTreeMeshes.Num(), RuntimeCC0PineTreeMeshes.Num(), RuntimeCC0BushMeshes.Num(),
            RuntimeCC0GroundCoverMeshes.Num(), RuntimeCC0WildflowerMeshes.Num());
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("CC0 forest source assets are not imported yet; run Scripts/import_cc0_forest_assets.py in Unreal Editor."));
    }

    const bool bHasAnyFabTrees = RuntimeFabTreeBroadleafMesh || RuntimeFabTreeSmallMesh || RuntimeFabPineMesh;
    const bool bHasAnyFabBushes = RuntimeFabBushLargeMesh || RuntimeFabBushMediumMesh || RuntimeFabBushFlowerMesh;
    if (bHasAnyFabTrees || bHasAnyFabBushes || RuntimeFabMansionMesh || RuntimeFabHorseMesh)
    {
        UE_LOG(LogTemp, Log, TEXT("Fab environment assets detected: trees=%s bushes=%s mansion=%s horse=%s."),
            bHasAnyFabTrees ? TEXT("yes") : TEXT("no"),
            bHasAnyFabBushes ? TEXT("yes") : TEXT("no"),
            RuntimeFabMansionMesh ? TEXT("yes") : TEXT("no"),
            RuntimeFabHorseMesh ? TEXT("yes") : TEXT("no"));
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Optional Fab assets are not imported; CC0 or built-in vegetation fallbacks remain available."));
    }
}

UMaterialInstanceDynamic* AAetherDevelopmentWorldActor::CreateColorMaterial(const FLinearColor& Color)
{
    if (!RuntimeBaseMaterial)
    {
        return nullptr;
    }

    const uint32 ColorKey = Color.ToFColor(true).DWColor();
    if (TObjectPtr<UMaterialInstanceDynamic>* Existing = RuntimeMaterials.Find(ColorKey))
    {
        return Existing->Get();
    }

    UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(RuntimeBaseMaterial, this);
    if (!Material)
    {
        return nullptr;
    }

    Material->SetVectorParameterValue(TEXT("Color"), Color);
    Material->SetScalarParameterValue(TEXT("Roughness"), 0.82f);
    Material->SetScalarParameterValue(TEXT("Specular"), 0.18f);
    Material->SetScalarParameterValue(TEXT("Metallic"), 0.0f);
    RuntimeMaterials.Add(ColorKey, Material);
    return Material;
}

UStaticMeshComponent* AAetherDevelopmentWorldActor::AddPrimitive(
    UStaticMesh* Mesh,
    const FName& Name,
    const FVector& Location,
    const FVector& Scale,
    const FLinearColor& Color,
    bool bBlockMovement,
    const FRotator& Rotation)
{
    if (!Mesh || !Root)
    {
        return nullptr;
    }

    const FName UniqueName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), Name);
    UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this, UniqueName);
    if (!Component)
    {
        return nullptr;
    }

    Component->SetupAttachment(Root);
    Component->SetStaticMesh(Mesh);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeRotation(Rotation);
    Component->SetRelativeScale3D(Scale);
    Component->SetMobility(EComponentMobility::Movable);
    Component->SetCollisionEnabled(
        bBlockMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    Component->SetCollisionProfileName(bBlockMovement ? TEXT("BlockAll") : TEXT("NoCollision"));
    Component->SetCanEverAffectNavigation(false);
    Component->SetCastShadow(true);

    if (UMaterialInstanceDynamic* Material = CreateColorMaterial(Color))
    {
        Component->SetMaterial(0, Material);
    }

    AddInstanceComponent(Component);
    Component->RegisterComponent();
    Component->SetVisibility(true, true);
    Component->MarkRenderStateDirty();
    RuntimeVisualComponents.Add(Component);
    return Component;
}

void AAetherDevelopmentWorldActor::AddInstancedPrimitive(
    UStaticMesh* Mesh,
    const FName& Name,
    const FVector& Location,
    const FVector& Scale,
    const FLinearColor& Color,
    bool bBlockMovement,
    const FRotator& Rotation,
    bool bCastShadow)
{
    if (!Mesh || !Root)
    {
        return;
    }

    const uint32 ColorKey = Color.ToFColor(true).DWColor();
    const uint32 ComponentKey = HashCombine(
        HashCombine(HashCombine(GetTypeHash(Mesh), ColorKey), bBlockMovement ? 1u : 0u),
        bCastShadow ? 1u : 0u);

    UInstancedStaticMeshComponent* Component = nullptr;
    if (TObjectPtr<UInstancedStaticMeshComponent>* Existing = RuntimeInstancedComponents.Find(ComponentKey))
    {
        Component = Existing->Get();
    }
    else
    {
        const FName UniqueName = MakeUniqueObjectName(
            this,
            UInstancedStaticMeshComponent::StaticClass(),
            FName(*FString::Printf(TEXT("%s_Instanced"), *Name.ToString())));
        Component = NewObject<UInstancedStaticMeshComponent>(this, UniqueName);
        if (!Component)
        {
            return;
        }

        Component->SetupAttachment(Root);
        Component->SetStaticMesh(Mesh);
        Component->SetMobility(EComponentMobility::Movable);
        Component->SetCollisionEnabled(
            bBlockMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
        Component->SetCollisionProfileName(bBlockMovement ? TEXT("BlockAll") : TEXT("NoCollision"));
        Component->SetCanEverAffectNavigation(false);
        Component->SetCastShadow(bCastShadow);

        if (UMaterialInstanceDynamic* Material = CreateColorMaterial(Color))
        {
            Component->SetMaterial(0, Material);
        }

        AddInstanceComponent(Component);
        Component->RegisterComponent();
        RuntimeInstancedComponents.Add(ComponentKey, Component);
        RuntimeVisualComponents.Add(Component);
    }

    if (Component)
    {
        Component->AddInstance(FTransform(Rotation, Location, Scale), false);
    }
}

void AAetherDevelopmentWorldActor::AddFoliageInstance(
    UStaticMesh* Mesh,
    const FName& BatchName,
    const FVector& GroundLocation,
    float TargetHeight,
    float WidthScale,
    const FRotator& Rotation)
{
    if (!Mesh || !Root || TargetHeight <= 0.0f)
    {
        return;
    }

    const FBox Bounds = Mesh->GetBoundingBox();
    const FVector MeshSize = Bounds.GetSize();
    if (!Bounds.IsValid || MeshSize.Z <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    UInstancedStaticMeshComponent* Component = nullptr;
    if (TObjectPtr<UInstancedStaticMeshComponent>* Existing = RuntimeFoliageInstancedComponents.Find(BatchName))
    {
        Component = Existing->Get();
        if (Component && Component->GetStaticMesh() != Mesh)
        {
            UE_LOG(LogTemp, Warning, TEXT("Foliage batch '%s' was reused with a different mesh; skipping instance."),
                *BatchName.ToString());
            return;
        }
    }
    else
    {
        const FName UniqueName = MakeUniqueObjectName(
            this, UInstancedStaticMeshComponent::StaticClass(),
            FName(*FString::Printf(TEXT("%s_Instances"), *BatchName.ToString())));
        Component = NewObject<UInstancedStaticMeshComponent>(this, UniqueName);
        if (!Component)
        {
            return;
        }

        Component->SetupAttachment(Root);
        Component->SetStaticMesh(Mesh);
        Component->SetMobility(EComponentMobility::Movable);
        Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Component->SetCollisionProfileName(TEXT("NoCollision"));
        Component->SetCanEverAffectNavigation(false);
        Component->SetCastShadow(true);
        AddInstanceComponent(Component);
        Component->RegisterComponent();
        RuntimeFoliageInstancedComponents.Add(BatchName, Component);
        RuntimeVisualComponents.Add(Component);
    }

    if (!Component)
    {
        return;
    }

    const float UniformScale = TargetHeight / MeshSize.Z;
    const FVector InstanceScale(
        UniformScale * WidthScale,
        UniformScale * WidthScale,
        UniformScale);
    const FVector Center = Bounds.GetCenter();
    FVector LocalOffset(-Center.X * InstanceScale.X, -Center.Y * InstanceScale.Y,
        -Bounds.Min.Z * InstanceScale.Z);
    const FVector InstanceLocation = GroundLocation + Rotation.RotateVector(LocalOffset);
    Component->AddInstance(FTransform(Rotation, InstanceLocation, InstanceScale), false);
}

void AAetherDevelopmentWorldActor::AddRibbon(
    const TArray<FVector>& Points,
    float Width,
    float Z,
    float Thickness,
    const FLinearColor& Color,
    const FName& NamePrefix,
    bool bRoundJoints)
{
    if (Points.Num() < 2 || Width <= 0.0f || Thickness <= 0.0f)
    {
        return;
    }

    const FString Prefix = NamePrefix.ToString();
    for (int32 Index = 0; Index < Points.Num() - 1; ++Index)
    {
        const FVector& Start = Points[Index];
        const FVector& End = Points[Index + 1];
        const FVector Delta = End - Start;
        const float Length = Delta.Size2D();
        if (Length <= 1.0f)
        {
            continue;
        }

        const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X));
        const FVector Center((Start.X + End.X) * 0.5f, (Start.Y + End.Y) * 0.5f, Z);
        const float Overlap = Width * 0.06f;
        AddInstancedPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_Segment_%02d"), *Prefix, Index)),
            Center,
            FVector((Length + Overlap) / 100.0f, Width / 100.0f, Thickness / 100.0f),
            Color,
            false,
            FRotator(0.0f, Yaw, 0.0f));
    }

    if (bRoundJoints)
    {
        for (int32 Index = 0; Index < Points.Num(); ++Index)
        {
            AddInstancedPrimitive(
                RuntimeSphereMesh,
                FName(*FString::Printf(TEXT("%s_Joint_%02d"), *Prefix, Index)),
                FVector(Points[Index].X, Points[Index].Y, Z),
                FVector(Width / 100.0f, Width / 100.0f, Thickness / 100.0f),
                Color);
        }
    }
}

void AAetherDevelopmentWorldActor::BuildHouse(
    const FVector& Location,
    float Scale,
    const FLinearColor& RoofColor,
    const FName& NamePrefix,
    float Yaw,
    const FLinearColor& WallColor)
{
    const FString Prefix = NamePrefix.ToString();
    const FQuat YawRotation = FRotator(0.0f, Yaw, 0.0f).Quaternion();
    const auto Local = [&Location, Scale, &YawRotation](const FVector& Value)
    {
        return Location + YawRotation.RotateVector(Value * Scale);
    };
    const FRotator HouseRotation(0.0f, Yaw, 0.0f);
    const FLinearColor WindowColor(0.22f, 0.62f, 0.82f, 1.0f);
    const FLinearColor DoorColor(0.22f, 0.12f, 0.07f, 1.0f);
    const FLinearColor TrimColor(0.28f, 0.16f, 0.09f, 1.0f);

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_Foundation"), *Prefix)),
        Local(FVector(0.0f, 0.0f, 22.0f)),
        FVector(4.9f * Scale, 3.9f * Scale, 0.42f * Scale),
        WarmStoneColor,
        true,
        HouseRotation);

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_PlasterWalls"), *Prefix)),
        Local(FVector(0.0f, 0.0f, 158.0f)),
        FVector(4.35f * Scale, 3.35f * Scale, 2.65f * Scale),
        WallColor,
        true,
        HouseRotation);

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_RoofSlope_Left"), *Prefix)),
        Local(FVector(0.0f, -78.0f, 315.0f)),
        FVector(5.05f * Scale, 2.42f * Scale, 0.25f * Scale),
        RoofColor,
        false,
        FRotator(0.0f, Yaw, 31.0f));

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_RoofSlope_Right"), *Prefix)),
        Local(FVector(0.0f, 78.0f, 315.0f)),
        FVector(5.05f * Scale, 2.42f * Scale, 0.25f * Scale),
        RoofColor * FLinearColor(0.80f, 0.80f, 0.84f, 1.0f),
        false,
        FRotator(0.0f, Yaw, -31.0f));

    const float RoofPitchRadians = FMath::DegreesToRadians(31.0f);
    for (int32 Side = -1; Side <= 1; Side += 2)
    {
        const float RoofRoll = -31.0f * static_cast<float>(Side);
        const float RoofCenterY = 78.0f * static_cast<float>(Side);
        const FRotator RoofRotation(0.0f, Yaw, RoofRoll);
        for (int32 Row = 0; Row < 5; ++Row)
        {
            const float TileY = static_cast<float>(Side) * (28.0f + 38.0f * Row);
            const float TileZ = 315.0f + FMath::Sin(-static_cast<float>(Side) * RoofPitchRadians) * (TileY - RoofCenterY) + 13.0f;
            for (int32 Column = 0; Column < 7; ++Column)
            {
                const float TileX = -180.0f + 60.0f * Column + (Row % 2 == 0 ? 0.0f : 30.0f);
                const FLinearColor TileColor = (Row + Column) % 3 == 0
                    ? RoofColor * FLinearColor(0.78f, 0.82f, 0.90f, 1.0f)
                    : ((Row + Column) % 3 == 1
                        ? RoofColor * FLinearColor(0.90f, 0.92f, 0.98f, 1.0f)
                        : RoofColor * FLinearColor(0.84f, 0.87f, 0.94f, 1.0f));
                AddInstancedPrimitive(
                    RuntimeCubeMesh,
                    FName(*FString::Printf(TEXT("%s_RoofTile_%d_%d_%d"), *Prefix, Side, Row, Column)),
                    Local(FVector(TileX, TileY, TileZ)),
                    FVector(0.58f * Scale, 0.37f * Scale, 0.08f * Scale),
                    TileColor,
                    false,
                    RoofRotation);
            }
        }
    }

    for (int32 Column = 0; Column < 7; ++Column)
    {
        const float TileX = -180.0f + 60.0f * Column;
        AddInstancedPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_RidgeCap_%d"), *Prefix, Column)),
            Local(FVector(TileX, 0.0f, 368.0f)),
            FVector(0.62f * Scale, 0.30f * Scale, 0.12f * Scale),
            RoofColor * FLinearColor(0.72f, 0.76f, 0.84f, 1.0f),
            false,
            HouseRotation);
    }

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_Ridge"), *Prefix)),
        Local(FVector(0.0f, 0.0f, 365.0f)),
        FVector(5.15f * Scale, 0.20f * Scale, 0.18f * Scale),
        RoofColor * FLinearColor(0.65f, 0.65f, 0.70f, 1.0f),
        false,
        HouseRotation);

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_Door"), *Prefix)),
        Local(FVector(0.0f, -171.0f, 88.0f)),
        FVector(0.58f * Scale, 0.12f * Scale, 1.62f * Scale),
        DoorColor,
        false,
        HouseRotation);

    AddPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_DoorHandle"), *Prefix)),
        Local(FVector(19.0f, -181.0f, 90.0f)),
        FVector(0.10f * Scale, 0.10f * Scale, 0.10f * Scale),
        FLinearColor(0.92f, 0.66f, 0.18f, 1.0f));

    for (int32 Side = -1; Side <= 1; Side += 2)
    {
        const float WindowX = 132.0f * static_cast<float>(Side);
        AddPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_Window_%d"), *Prefix, Side)),
            Local(FVector(WindowX, -171.0f, 193.0f)),
            FVector(0.76f * Scale, 0.12f * Scale, 0.78f * Scale),
            WindowColor,
            false,
            HouseRotation);
        AddPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_WindowMullion_%d"), *Prefix, Side)),
            Local(FVector(WindowX, -179.0f, 193.0f)),
            FVector(0.10f * Scale, 0.08f * Scale, 0.74f * Scale),
            TrimColor);
        AddPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_FrontBeam_%d"), *Prefix, Side)),
            Local(FVector(210.0f * static_cast<float>(Side), -170.0f, 158.0f)),
            FVector(0.14f * Scale, 0.16f * Scale, 2.75f * Scale),
            TrimColor,
            false,
            HouseRotation);
    }

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_FrontCrossbeam"), *Prefix)),
        Local(FVector(0.0f, -170.0f, 278.0f)),
        FVector(4.42f * Scale, 0.15f * Scale, 0.18f * Scale),
        TrimColor,
        false,
        HouseRotation);

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_PorchStep"), *Prefix)),
        Local(FVector(0.0f, -222.0f, 17.0f)),
        FVector(1.35f * Scale, 0.58f * Scale, 0.28f * Scale),
        StoneColor,
        false,
        HouseRotation);

    AddPrimitive(
        RuntimeCylinderMesh,
        FName(*FString::Printf(TEXT("%s_Chimney"), *Prefix)),
        Local(FVector(150.0f, 55.0f, 390.0f)),
        FVector(0.50f * Scale, 0.50f * Scale, 1.0f * Scale),
        FLinearColor(0.53f, 0.27f, 0.18f, 1.0f),
        false,
        HouseRotation);
}

void AAetherDevelopmentWorldActor::BuildTower(
    const FVector& Location,
    float Height,
    float Radius,
    const FLinearColor& RoofColor,
    const FName& NamePrefix)
{
    const FString Prefix = NamePrefix.ToString();
    const float DiameterScale = Radius * 2.0f / 100.0f;
    const float RoofHeight = Height * 0.36f;

    AddInstancedPrimitive(
        RuntimeCylinderMesh,
        FName(*FString::Printf(TEXT("%s_StoneShaft"), *Prefix)),
        Location + FVector(0.0f, 0.0f, Height * 0.5f),
        FVector(DiameterScale, DiameterScale, Height / 100.0f),
        FLinearColor(0.80f, 0.79f, 0.70f, 1.0f),
        true);

    AddInstancedPrimitive(
        RuntimeCylinderMesh,
        FName(*FString::Printf(TEXT("%s_CrownBand"), *Prefix)),
        Location + FVector(0.0f, 0.0f, Height - 18.0f),
        FVector(DiameterScale * 1.12f, DiameterScale * 1.12f, 0.24f),
        WarmStoneColor,
        false);

    AddInstancedPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_Window"), *Prefix)),
        Location + FVector(0.0f, -Radius - 4.0f, Height * 0.64f),
        FVector(0.34f, 0.11f, 0.92f),
        FLinearColor(0.22f, 0.47f, 0.65f, 1.0f));

    AddInstancedPrimitive(
        RuntimeConeMesh,
        FName(*FString::Printf(TEXT("%s_Roof"), *Prefix)),
        Location + FVector(0.0f, 0.0f, Height + RoofHeight * 0.48f),
        FVector(DiameterScale * 1.55f, DiameterScale * 1.55f, RoofHeight / 100.0f),
        RoofColor);

    const float RoofBaseRadius = Radius * 1.55f;
    const float RoofBaseZ = Height + RoofHeight * 0.48f - RoofHeight * 0.5f;
    for (int32 Row = 0; Row < 4; ++Row)
    {
        const float RoofFraction = 0.10f + 0.22f * Row;
        const float RingRadius = RoofBaseRadius * (1.0f - RoofFraction);
        const int32 TileCount = FMath::Max(3, FMath::CeilToInt(2.0f * PI * RingRadius / 52.0f));
        for (int32 TileIndex = 0; TileIndex < TileCount; ++TileIndex)
        {
            const float Angle = 2.0f * PI * (static_cast<float>(TileIndex) + (Row % 2 == 0 ? 0.0f : 0.5f))
                / static_cast<float>(TileCount);
            const FVector Radial(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
            const FVector SurfaceNormal = FVector(Radial.X, Radial.Y, RoofBaseRadius / RoofHeight).GetSafeNormal();
            const FVector TileLocation = Location
                + Radial * RingRadius
                + FVector(0.0f, 0.0f, RoofBaseZ + RoofHeight * RoofFraction)
                + SurfaceNormal * 4.0f;
            const FLinearColor TileColor = (TileIndex + Row) % 3 == 0
                ? RoofColor * FLinearColor(0.79f, 0.84f, 0.94f, 1.0f)
                : ((TileIndex + Row) % 3 == 1
                    ? RoofColor * FLinearColor(0.95f, 0.96f, 1.0f, 1.0f)
                    : RoofColor * FLinearColor(0.86f, 0.90f, 0.97f, 1.0f));
            AddInstancedPrimitive(
                RuntimeCubeMesh,
                FName(*FString::Printf(TEXT("%s_RoofTile_%d_%d"), *Prefix, Row, TileIndex)),
                TileLocation,
                FVector(0.42f, 0.48f, 0.06f),
                TileColor,
                false,
                FRotationMatrix::MakeFromZ(SurfaceNormal).Rotator());
        }
    }

    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_Finial"), *Prefix)),
        Location + FVector(0.0f, 0.0f, Height + RoofHeight + 8.0f),
        FVector(0.13f, 0.13f, 0.18f),
        FLinearColor(0.96f, 0.70f, 0.20f, 1.0f));
}

void AAetherDevelopmentWorldActor::BuildTree(
    const FVector& Location,
    float Scale,
    int32 Variant,
    const FName& NamePrefix)
{
    if (RuntimeCC0BroadleafTreeMeshes.Num() > 0 || RuntimeCC0PineTreeMeshes.Num() > 0)
    {
        const bool bPine = Variant % 3 == 1;
        const bool bSmall = !bPine && Variant % 2 == 0;
        const uint32 SelectionSeed = GetTypeHash(NamePrefix);
        const auto SelectMesh = [SelectionSeed](const TArray<TObjectPtr<UStaticMesh>>& Meshes) -> UStaticMesh*
        {
            if (Meshes.Num() <= 0)
            {
                return nullptr;
            }
            const int32 MeshIndex = static_cast<int32>(SelectionSeed % static_cast<uint32>(Meshes.Num()));
            return Meshes[MeshIndex].Get();
        };

        UStaticMesh* CC0Tree = bPine
            ? SelectMesh(RuntimeCC0PineTreeMeshes)
            : SelectMesh(RuntimeCC0BroadleafTreeMeshes);
        if (!CC0Tree)
        {
            CC0Tree = SelectMesh(bPine ? RuntimeCC0BroadleafTreeMeshes : RuntimeCC0PineTreeMeshes);
        }
        if (CC0Tree)
        {
            const FName BatchName(*FString::Printf(TEXT("CC0_Tree_%s"), *CC0Tree->GetName()));
            const float TargetHeight = (bPine ? 700.0f : (bSmall ? 420.0f : 560.0f)) * Scale;
            const float WidthScale = 0.90f + static_cast<float>(SelectionSeed % 24u) / 100.0f;
            const float Yaw = static_cast<float>(SelectionSeed % 360u);
            AddFoliageInstance(CC0Tree, BatchName, Location, TargetHeight, WidthScale, FRotator(0.0f, Yaw, 0.0f));
            return;
        }
    }

    if (RuntimeFabTreeBroadleafMesh || RuntimeFabTreeSmallMesh || RuntimeFabPineMesh)
    {
        const bool bPine = Variant % 3 == 1;
        const bool bUseSmall = !bPine && (Variant % 2 == 0) && RuntimeFabTreeSmallMesh;
        UStaticMesh* FabTree = bPine && RuntimeFabPineMesh
            ? RuntimeFabPineMesh.Get()
            : (bUseSmall ? RuntimeFabTreeSmallMesh.Get() : RuntimeFabTreeBroadleafMesh.Get());
        if (!FabTree)
        {
            FabTree = RuntimeFabTreeSmallMesh ? RuntimeFabTreeSmallMesh.Get()
                : (RuntimeFabPineMesh ? RuntimeFabPineMesh.Get() : RuntimeFabTreeBroadleafMesh.Get());
        }
        if (FabTree)
        {
            const FName BatchName = FabTree == RuntimeFabPineMesh.Get() ? TEXT("FabPineInstances")
                : (FabTree == RuntimeFabTreeSmallMesh.Get() ? TEXT("FabSmallTreeInstances") : TEXT("FabBroadleafInstances"));
            const float TargetHeight = (bPine ? 700.0f : (bUseSmall ? 420.0f : 560.0f)) * Scale;
            const float WidthScale = 0.88f + static_cast<float>(GetTypeHash(NamePrefix) % 25u) / 100.0f;
            const float Yaw = static_cast<float>(GetTypeHash(NamePrefix) % 360u);
            AddFoliageInstance(FabTree, BatchName, Location, TargetHeight, WidthScale, FRotator(0.0f, Yaw, 0.0f));
            return;
        }
    }

    const FString Prefix = NamePrefix.ToString();
    const float Height = (Variant % 3 == 1 ? 540.0f : 450.0f) * Scale;
    const float TrunkScale = 0.42f * Scale;
    const FLinearColor Bark(0.32f, 0.18f, 0.09f, 1.0f);
    const FLinearColor LeafDark(0.12f, 0.32f, 0.13f, 1.0f);
    const FLinearColor LeafMid(0.20f, 0.46f, 0.16f, 1.0f);
    const FLinearColor LeafLight(0.36f, 0.59f, 0.20f, 1.0f);
    const bool bPine = Variant % 3 == 1;
    const bool bBlossom = Variant % 5 == 4;

    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_GroundShadow"), *Prefix)),
        Location + FVector(24.0f * Scale, -16.0f * Scale, 4.0f),
        FVector(2.55f * Scale, 1.85f * Scale, 0.05f),
        FLinearColor(0.15f, 0.28f, 0.12f, 1.0f));

    AddInstancedPrimitive(
        RuntimeCylinderMesh,
        FName(*FString::Printf(TEXT("%s_Trunk"), *Prefix)),
        Location + FVector(0.0f, 0.0f, Height * 0.32f),
        FVector(TrunkScale, TrunkScale, Height * 0.66f / 100.0f),
        Bark,
        true);

    if (bPine)
    {
        const FVector PineBranchStart = Location + FVector(0.0f, 0.0f, Height * 0.38f);
        for (int32 BranchIndex = 0; BranchIndex < 4; ++BranchIndex)
        {
            const float BranchAngle = FMath::DegreesToRadians(45.0f + 90.0f * BranchIndex);
            const FVector BranchEnd = Location + FVector(
                FMath::Cos(BranchAngle) * 148.0f * Scale,
                FMath::Sin(BranchAngle) * 148.0f * Scale,
                Height * (0.62f + 0.04f * BranchIndex));
            const FVector BranchDelta = BranchEnd - PineBranchStart;
            AddInstancedPrimitive(
                RuntimeCylinderMesh,
                FName(*FString::Printf(TEXT("%s_PineBranch_%d"), *Prefix, BranchIndex)),
                (PineBranchStart + BranchEnd) * 0.5f,
                FVector(0.12f * Scale, 0.12f * Scale, BranchDelta.Size() / 100.0f),
                Bark,
                false,
                FRotationMatrix::MakeFromZ(BranchDelta.GetSafeNormal()).Rotator());
            AddInstancedPrimitive(
                RuntimeSphereMesh,
                FName(*FString::Printf(TEXT("%s_PineBranchNeedles_%d"), *Prefix, BranchIndex)),
                BranchEnd + FVector(0.0f, 0.0f, 7.0f * Scale),
                FVector(0.42f * Scale, 0.34f * Scale, 0.30f * Scale),
                BranchIndex % 2 == 0 ? LeafMid : LeafLight);
        }

        for (int32 Layer = 0; Layer < 3; ++Layer)
        {
            const float LayerHeight = Height * (0.52f + 0.17f * Layer);
            const float LayerWidth = (2.65f - 0.52f * Layer) * Scale;
            AddInstancedPrimitive(
                RuntimeConeMesh,
                FName(*FString::Printf(TEXT("%s_PineLayer_%d"), *Prefix, Layer)),
                Location + FVector(0.0f, 0.0f, LayerHeight),
                FVector(LayerWidth, LayerWidth, (2.35f - 0.20f * Layer) * Scale),
                Layer == 0 ? LeafDark : (Layer == 1 ? LeafMid : LeafLight));
        }
        return;
    }

    const FLinearColor CanopyMain = bBlossom
        ? FLinearColor(0.55f, 0.24f, 0.46f, 1.0f)
        : LeafMid;
    const FLinearColor CanopyAccent = bBlossom
        ? FLinearColor(0.83f, 0.43f, 0.66f, 1.0f)
        : LeafLight;
    const FLinearColor CanopyShadow = bBlossom
        ? FLinearColor(0.34f, 0.16f, 0.32f, 1.0f)
        : LeafDark;

    struct FCanopyBranch
    {
        FVector EndOffset;
        float BranchRadius;
    };
    const FCanopyBranch Branches[] = {
        { FVector(-245.0f, -18.0f, Height * 0.77f), 0.15f },
        { FVector(238.0f, 24.0f, Height * 0.76f), 0.15f },
        { FVector(-24.0f, 238.0f, Height * 0.80f), 0.13f },
        { FVector(18.0f, -232.0f, Height * 0.79f), 0.13f }
    };
    const FVector BranchStart = Location + FVector(0.0f, 0.0f, Height * 0.43f);
    for (int32 BranchIndex = 0; BranchIndex < UE_ARRAY_COUNT(Branches); ++BranchIndex)
    {
        const FVector BranchEnd = Location + Branches[BranchIndex].EndOffset * Scale;
        const FVector BranchDelta = BranchEnd - BranchStart;
        const float BranchLength = BranchDelta.Size();
        const FRotator BranchRotation = FRotationMatrix::MakeFromZ(BranchDelta.GetSafeNormal()).Rotator();
        AddInstancedPrimitive(
            RuntimeCylinderMesh,
            FName(*FString::Printf(TEXT("%s_Branch_%d"), *Prefix, BranchIndex)),
            (BranchStart + BranchEnd) * 0.5f,
            FVector(Branches[BranchIndex].BranchRadius * Scale, Branches[BranchIndex].BranchRadius * Scale, BranchLength / 100.0f),
            Bark,
            false,
            BranchRotation);
    }

    const FVector CanopyOffsets[] = {
        FVector(-112.0f, -16.0f, Height * 0.70f),
        FVector(108.0f, 18.0f, Height * 0.71f),
        FVector(-12.0f, -112.0f, Height * 0.75f),
        FVector(20.0f, 108.0f, Height * 0.76f),
        FVector(-62.0f, 50.0f, Height * 0.91f),
        FVector(68.0f, -48.0f, Height * 0.92f),
        FVector(-4.0f, 8.0f, Height * 1.08f),
        FVector(8.0f, -26.0f, Height * 0.59f)
    };
    const FVector CanopyScales[] = {
        FVector(1.70f, 1.52f, 1.60f), FVector(1.68f, 1.55f, 1.62f),
        FVector(1.62f, 1.50f, 1.58f), FVector(1.66f, 1.56f, 1.60f),
        FVector(1.50f, 1.40f, 1.48f), FVector(1.48f, 1.38f, 1.50f),
        FVector(1.40f, 1.34f, 1.42f), FVector(1.46f, 1.38f, 1.34f)
    };
    const FLinearColor CanopyColors[] = {
        CanopyShadow, CanopyMain, CanopyAccent, CanopyMain,
        CanopyAccent, CanopyMain * FLinearColor(0.91f, 1.0f, 0.88f, 1.0f),
        CanopyAccent, CanopyShadow
    };
    for (int32 ClusterIndex = 0; ClusterIndex < UE_ARRAY_COUNT(CanopyOffsets); ++ClusterIndex)
    {
        AddInstancedPrimitive(
            RuntimeSphereMesh,
            FName(*FString::Printf(TEXT("%s_LeafCluster_%d"), *Prefix, ClusterIndex)),
            Location + CanopyOffsets[ClusterIndex] * Scale,
            CanopyScales[ClusterIndex] * Scale,
            CanopyColors[ClusterIndex]);
    }

    const FVector LeafOffsets[] = {
        FVector(-224.0f, -35.0f, Height * 0.76f), FVector(220.0f, 29.0f, Height * 0.78f),
        FVector(-22.0f, -220.0f, Height * 0.81f), FVector(18.0f, 216.0f, Height * 0.82f),
        FVector(-112.0f, 106.0f, Height * 1.02f), FVector(119.0f, -108.0f, Height * 1.01f),
        FVector(-135.0f, -104.0f, Height * 0.96f), FVector(133.0f, 102.0f, Height * 0.95f)
    };
    for (int32 LeafIndex = 0; LeafIndex < UE_ARRAY_COUNT(LeafOffsets); ++LeafIndex)
    {
        const FLinearColor LeafColor = LeafIndex % 3 == 0
            ? CanopyAccent
            : (LeafIndex % 3 == 1 ? CanopyMain : CanopyMain * FLinearColor(0.92f, 1.0f, 0.90f, 1.0f));
        AddInstancedPrimitive(
            RuntimeSphereMesh,
            FName(*FString::Printf(TEXT("%s_Foliage_%d"), *Prefix, LeafIndex)),
            Location + LeafOffsets[LeafIndex] * Scale,
            FVector(0.56f * Scale, 0.40f * Scale, 0.38f * Scale),
            LeafColor,
            false,
            FRotator(
                static_cast<float>((LeafIndex * 13) % 24 - 12),
                static_cast<float>(LeafIndex * 41),
                static_cast<float>((LeafIndex * 17) % 28 - 14)));
    }
}

void AAetherDevelopmentWorldActor::BuildRockCluster(
    const FVector& Location,
    float Scale,
    const FName& NamePrefix)
{
    const FString Prefix = NamePrefix.ToString();
    const FLinearColor RockDark(0.34f, 0.37f, 0.35f, 1.0f);
    const FLinearColor RockMid(0.55f, 0.57f, 0.51f, 1.0f);
    const FLinearColor RockLight(0.72f, 0.70f, 0.59f, 1.0f);

    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_MainBoulder"), *Prefix)),
        Location + FVector(0.0f, 0.0f, 54.0f * Scale),
        FVector(2.35f * Scale, 1.78f * Scale, 1.35f * Scale),
        RockMid,
        true,
        FRotator(0.0f, 16.0f, -8.0f));
    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_SideBoulder"), *Prefix)),
        Location + FVector(88.0f * Scale, 32.0f * Scale, 35.0f * Scale),
        FVector(1.45f * Scale, 1.18f * Scale, 0.82f * Scale),
        RockDark,
        true,
        FRotator(4.0f, 28.0f, 12.0f));
    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_LitFace"), *Prefix)),
        Location + FVector(-35.0f * Scale, -52.0f * Scale, 93.0f * Scale),
        FVector(0.92f * Scale, 0.68f * Scale, 0.56f * Scale),
        RockLight);
}

void AAetherDevelopmentWorldActor::BuildFarmPlot(
    const FVector& Center,
    float Width,
    float Depth,
    float Yaw,
    const FLinearColor& SoilColor,
    const FLinearColor& CropColor,
    const FName& NamePrefix)
{
    const FString Prefix = NamePrefix.ToString();
    const FRotator PlotRotation(0.0f, Yaw, 0.0f);
    const FQuat PlotQuaternion = PlotRotation.Quaternion();
    const auto Local = [&Center, &PlotQuaternion](const FVector& Value)
    {
        return Center + PlotQuaternion.RotateVector(Value);
    };

    AddPrimitive(
        RuntimeCubeMesh,
        FName(*FString::Printf(TEXT("%s_SoilBed"), *Prefix)),
        Center + FVector(0.0f, 0.0f, 5.0f),
        FVector(Width / 100.0f, Depth / 100.0f, 0.10f),
        SoilColor,
        false,
        PlotRotation);

    const int32 RowCount = 8;
    const int32 PlantCount = 6;
    for (int32 Row = 0; Row < RowCount; ++Row)
    {
        const float LocalY = -Depth * 0.40f + Depth * 0.115f * static_cast<float>(Row);
        const FLinearColor RowColor = (Row % 2 == 0)
            ? CropColor
            : CropColor * FLinearColor(0.78f, 0.88f, 0.72f, 1.0f);

        AddInstancedPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_Furrow_%02d"), *Prefix, Row)),
            Local(FVector(0.0f, LocalY, 17.0f)),
            FVector((Width - 42.0f) / 100.0f, 0.085f, 0.16f),
            RowColor,
            false,
            PlotRotation);

        for (int32 Plant = 0; Plant < PlantCount; ++Plant)
        {
            const float LocalX = -Width * 0.39f + (Width * 0.78f / static_cast<float>(PlantCount - 1)) * Plant;
            const float PlantHeight = 22.0f + static_cast<float>((Row + Plant) % 3) * 5.0f;
            AddInstancedPrimitive(
                RuntimeConeMesh,
                FName(*FString::Printf(TEXT("%s_Crop_%02d_%02d"), *Prefix, Row, Plant)),
                Local(FVector(LocalX, LocalY, PlantHeight)),
                FVector(0.24f, 0.24f, 0.38f + static_cast<float>((Row + Plant) % 2) * 0.10f),
                RowColor * FLinearColor(0.88f, 1.0f, 0.78f, 1.0f));
        }
    }

    const float HalfWidth = Width * 0.5f;
    const float HalfDepth = Depth * 0.5f;
    const FVector P0 = Local(FVector(-HalfWidth, -HalfDepth, 0.0f));
    const FVector P1 = Local(FVector(HalfWidth, -HalfDepth, 0.0f));
    const FVector P2 = Local(FVector(HalfWidth, HalfDepth, 0.0f));
    const FVector P3 = Local(FVector(-HalfWidth, HalfDepth, 0.0f));
    const float FenceHeight = 84.0f;
    BuildFence(P0, P1, FenceHeight, FName(*FString::Printf(TEXT("%s_Fence_South"), *Prefix)), TimberColor);
    BuildFence(P1, P2, FenceHeight, FName(*FString::Printf(TEXT("%s_Fence_East"), *Prefix)), TimberColor);
    BuildFence(P2, P3, FenceHeight, FName(*FString::Printf(TEXT("%s_Fence_North"), *Prefix)), TimberColor);
    BuildFence(P3, P0, FenceHeight, FName(*FString::Printf(TEXT("%s_Fence_West"), *Prefix)), TimberColor);
}

void AAetherDevelopmentWorldActor::BuildFence(
    const FVector& Start,
    const FVector& End,
    float Height,
    const FName& NamePrefix,
    const FLinearColor& Color)
{
    const FVector Delta = End - Start;
    const float Length = Delta.Size2D();
    if (Length <= 1.0f)
    {
        return;
    }

    const FString Prefix = NamePrefix.ToString();
    const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X));
    const FRotator FenceRotation(0.0f, Yaw, 0.0f);
    const FVector Center((Start.X + End.X) * 0.5f, (Start.Y + End.Y) * 0.5f, 0.0f);

    for (int32 Rail = 0; Rail < 2; ++Rail)
    {
        const float RailZ = Height * (Rail == 0 ? 0.40f : 0.84f);
        AddInstancedPrimitive(
            RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("%s_Rail_%d"), *Prefix, Rail)),
            Center + FVector(0.0f, 0.0f, RailZ),
            FVector(Length / 100.0f, 0.13f, 0.12f),
            Color,
            false,
            FenceRotation);
    }

    const int32 PostCount = FMath::Max(2, FMath::CeilToInt(Length / 170.0f));
    for (int32 Index = 0; Index <= PostCount; ++Index)
    {
        const float Alpha = static_cast<float>(Index) / static_cast<float>(PostCount);
        const FVector PostLocation = FMath::Lerp(Start, End, Alpha) + FVector(0.0f, 0.0f, Height * 0.5f);
        AddInstancedPrimitive(
            RuntimeCylinderMesh,
            FName(*FString::Printf(TEXT("%s_Post_%02d"), *Prefix, Index)),
            PostLocation,
            FVector(0.15f, 0.15f, Height / 100.0f),
            Color,
            false);
    }
}

void AAetherDevelopmentWorldActor::BuildMarketStall(
    const FVector& Location,
    const FLinearColor& CanopyColor,
    const FName& NamePrefix)
{
    const FString Prefix = NamePrefix.ToString();
    const FLinearColor Wood(0.36f, 0.20f, 0.10f, 1.0f);
    const FLinearColor CanvasLight(0.92f, 0.82f, 0.60f, 1.0f);

    AddPrimitive(RuntimeCubeMesh, FName(*FString::Printf(TEXT("%s_Counter"), *Prefix)),
        Location + FVector(0.0f, 0.0f, 62.0f), FVector(1.75f, 1.12f, 0.45f), Wood);
    AddPrimitive(RuntimeCubeMesh, FName(*FString::Printf(TEXT("%s_CounterTop"), *Prefix)),
        Location + FVector(0.0f, -2.0f, 88.0f), FVector(1.90f, 1.22f, 0.12f), WarmStoneColor);
    AddPrimitive(RuntimeCubeMesh, FName(*FString::Printf(TEXT("%s_Canopy"), *Prefix)),
        Location + FVector(0.0f, 0.0f, 178.0f), FVector(2.15f, 1.62f, 0.20f), CanopyColor);
    AddPrimitive(RuntimeCubeMesh, FName(*FString::Printf(TEXT("%s_CanopyStripe"), *Prefix)),
        Location + FVector(0.0f, -82.0f, 179.0f), FVector(1.95f, 0.13f, 0.22f), CanvasLight);

    for (int32 XSign = -1; XSign <= 1; XSign += 2)
    {
        for (int32 YSign = -1; YSign <= 1; YSign += 2)
        {
            AddPrimitive(RuntimeCylinderMesh,
                FName(*FString::Printf(TEXT("%s_Post_%d_%d"), *Prefix, XSign, YSign)),
                Location + FVector(74.0f * XSign, 58.0f * YSign, 87.0f),
                FVector(0.13f, 0.13f, 1.72f), Wood);
        }
    }

    AddPrimitive(RuntimeCubeMesh, FName(*FString::Printf(TEXT("%s_Crate"), *Prefix)),
        Location + FVector(-75.0f, -108.0f, 36.0f), FVector(0.44f, 0.42f, 0.52f),
        FLinearColor(0.56f, 0.34f, 0.16f, 1.0f));
}

void AAetherDevelopmentWorldActor::BuildLantern(const FVector& Location, const FName& NamePrefix)
{
    const FString Prefix = NamePrefix.ToString();
    const FLinearColor DarkIron(0.16f, 0.18f, 0.20f, 1.0f);
    AddPrimitive(RuntimeCylinderMesh, FName(*FString::Printf(TEXT("%s_Pole"), *Prefix)),
        Location + FVector(0.0f, 0.0f, 118.0f), FVector(0.11f, 0.11f, 2.36f), DarkIron);
    AddPrimitive(RuntimeSphereMesh, FName(*FString::Printf(TEXT("%s_LanternGlow"), *Prefix)),
        Location + FVector(0.0f, 0.0f, 244.0f), FVector(0.40f, 0.40f, 0.50f),
        FLinearColor(1.0f, 0.70f, 0.22f, 1.0f));
    AddPrimitive(RuntimeConeMesh, FName(*FString::Printf(TEXT("%s_Cap"), *Prefix)),
        Location + FVector(0.0f, 0.0f, 276.0f), FVector(0.50f, 0.50f, 0.28f), DarkIron);
}

void AAetherDevelopmentWorldActor::BuildTerrain()
{
    if (Ground)
    {
        if (RuntimeGrassGroundMaterial)
        {
            Ground->SetMaterial(0, RuntimeGrassGroundMaterial);
        }
        else if (UMaterialInstanceDynamic* FallbackMaterial = CreateColorMaterial(GrassColor))
        {
            Ground->SetMaterial(0, FallbackMaterial);
            UE_LOG(LogTemp, Warning, TEXT("Grass ground material is not imported; using the solid-color fallback. Run Scripts/import_fab_library_assets.py to import the textured ground."));
        }
    }

    struct FGroundPatch
    {
        FVector Location;
        FVector Scale;
        FLinearColor Color;
    };

    const FGroundPatch Patches[] = {
        { FVector(-1980.0f, 1150.0f, 2.0f), FVector(5.2f, 3.5f, 0.06f), LightGrassColor },
        { FVector(-1380.0f, -1450.0f, 2.0f), FVector(4.2f, 2.7f, 0.05f), DeepGrassColor },
        { FVector(1950.0f, -1350.0f, 2.0f), FVector(4.7f, 3.1f, 0.05f), LightGrassColor },
        { FVector(1850.0f, 1550.0f, 2.0f), FVector(5.0f, 3.0f, 0.05f), DeepGrassColor },
        { FVector(-450.0f, 1550.0f, 2.0f), FVector(4.1f, 2.6f, 0.05f), LightGrassColor },
        { FVector(420.0f, -1320.0f, 2.0f), FVector(3.8f, 2.5f, 0.05f), DeepGrassColor },
        { FVector(1600.0f, 300.0f, 2.0f), FVector(3.9f, 2.7f, 0.05f), LightGrassColor },
        { FVector(-2180.0f, -220.0f, 2.0f), FVector(3.0f, 2.2f, 0.05f), DeepGrassColor },
        { FVector(350.0f, 1450.0f, 2.0f), FVector(4.0f, 2.7f, 0.05f), LightGrassColor },
        { FVector(-1020.0f, 950.0f, 2.0f), FVector(3.6f, 2.5f, 0.05f), DeepGrassColor },
        { FVector(2050.0f, -850.0f, 2.0f), FVector(3.3f, 2.3f, 0.05f), LightGrassColor },
        { FVector(-500.0f, -1650.0f, 2.0f), FVector(3.8f, 2.4f, 0.05f), LightGrassColor }
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(Patches); ++Index)
    {
        AddInstancedPrimitive(
            RuntimeSphereMesh,
            FName(*FString::Printf(TEXT("MeadowColorPatch_%02d"), Index)),
            Patches[Index].Location,
            Patches[Index].Scale,
            Patches[Index].Color);
    }

    const FLinearColor HillGrass(0.33f, 0.52f, 0.22f, 1.0f);
    AddInstancedPrimitive(RuntimeSphereMesh, TEXT("NorthWestLowHill"), FVector(-2180.0f, 1420.0f, 34.0f),
        FVector(5.7f, 4.2f, 0.68f), HillGrass);
    AddInstancedPrimitive(RuntimeSphereMesh, TEXT("NorthEastLowHill"), FVector(2180.0f, 1450.0f, 48.0f),
        FVector(5.5f, 4.4f, 0.92f), HillGrass * FLinearColor(0.92f, 1.02f, 0.86f, 1.0f));
    AddInstancedPrimitive(RuntimeSphereMesh, TEXT("SouthWestLowHill"), FVector(-2200.0f, -1470.0f, 36.0f),
        FVector(5.0f, 3.8f, 0.76f), HillGrass * FLinearColor(0.90f, 0.98f, 0.86f, 1.0f));
    AddInstancedPrimitive(RuntimeSphereMesh, TEXT("SouthEastLowHill"), FVector(2250.0f, -1510.0f, 42.0f),
        FVector(5.4f, 4.1f, 0.82f), HillGrass);

    BuildRockCluster(FVector(-2310.0f, 1500.0f, 0.0f), 1.25f, TEXT("NorthWestRocks"));
    BuildRockCluster(FVector(2350.0f, 1240.0f, 0.0f), 1.1f, TEXT("NorthEastRocks"));
    BuildRockCluster(FVector(-2250.0f, -1450.0f, 0.0f), 1.0f, TEXT("SouthWestRocks"));
}

void AAetherDevelopmentWorldActor::BuildGroundCover()
{
    const FLinearColor BladeColors[] = {
        FLinearColor(0.24f, 0.42f, 0.14f, 1.0f),
        FLinearColor(0.31f, 0.50f, 0.17f, 1.0f),
        FLinearColor(0.39f, 0.58f, 0.22f, 1.0f),
        FLinearColor(0.29f, 0.46f, 0.16f, 1.0f)
    };
    const FLinearColor FlowerColors[] = {
        FLinearColor(0.96f, 0.78f, 0.24f, 1.0f),
        FLinearColor(0.95f, 0.92f, 0.78f, 1.0f),
        FLinearColor(0.78f, 0.48f, 0.76f, 1.0f)
    };
    const FName BladeInstanceNames[] = {
        TEXT("MeadowGrassBlade_Dark"), TEXT("MeadowGrassBlade_Mid"),
        TEXT("MeadowGrassBlade_Light"), TEXT("MeadowGrassBlade_Green")
    };
    const FName FlowerInstanceNames[] = {
        TEXT("MeadowWildflower_Gold"), TEXT("MeadowWildflower_Cream"), TEXT("MeadowWildflower_Lilac")
    };

    const TArray<FVector> RiverPath = {
        FVector(-2660.0f, 1720.0f, 0.0f), FVector(-2180.0f, 1410.0f, 0.0f),
        FVector(-1640.0f, 1130.0f, 0.0f), FVector(-1030.0f, 870.0f, 0.0f),
        FVector(-470.0f, 660.0f, 0.0f), FVector(120.0f, 490.0f, 0.0f),
        FVector(700.0f, 230.0f, 0.0f), FVector(1330.0f, -90.0f, 0.0f),
        FVector(1950.0f, -400.0f, 0.0f), FVector(2660.0f, -750.0f, 0.0f)
    };
    const TArray<FVector> MainRoad = {
        FVector(0.0f, -1740.0f, 0.0f), FVector(-40.0f, -1190.0f, 0.0f),
        FVector(-130.0f, -700.0f, 0.0f), FVector(-80.0f, -280.0f, 0.0f),
        FVector(0.0f, 0.0f, 0.0f), FVector(-60.0f, 280.0f, 0.0f),
        FVector(120.0f, 490.0f, 0.0f), FVector(350.0f, 720.0f, 0.0f),
        FVector(420.0f, 540.0f, 0.0f), FVector(700.0f, 520.0f, 0.0f),
        FVector(970.0f, 580.0f, 0.0f), FVector(1050.0f, 695.0f, 0.0f),
        FVector(1050.0f, 1080.0f, 0.0f)
    };
    const TArray<FVector> FarmRoad = {
        FVector(-120.0f, -270.0f, 0.0f), FVector(-430.0f, -365.0f, 0.0f),
        FVector(-780.0f, -425.0f, 0.0f), FVector(-1160.0f, -500.0f, 0.0f),
        FVector(-1560.0f, -565.0f, 0.0f), FVector(-1930.0f, -780.0f, 0.0f)
    };
    const TArray<FVector> OrchardPath = {
        FVector(-450.0f, 100.0f, 0.0f), FVector(-760.0f, 380.0f, 0.0f),
        FVector(-1160.0f, 640.0f, 0.0f), FVector(-1530.0f, 810.0f, 0.0f)
    };
    const TArray<TArray<FVector>> Paths = { RiverPath, MainRoad, FarmRoad, OrchardPath };

    const auto IsNearPath = [](const FVector2D& Point, const TArray<FVector>& Path, float Radius)
    {
        const float RadiusSquared = Radius * Radius;
        for (int32 SegmentIndex = 0; SegmentIndex < Path.Num() - 1; ++SegmentIndex)
        {
            const FVector& Start = Path[SegmentIndex];
            const FVector& End = Path[SegmentIndex + 1];
            const float DeltaX = End.X - Start.X;
            const float DeltaY = End.Y - Start.Y;
            const float LengthSquared = DeltaX * DeltaX + DeltaY * DeltaY;
            const float Alpha = LengthSquared > SMALL_NUMBER
                ? FMath::Clamp(((Point.X - Start.X) * DeltaX + (Point.Y - Start.Y) * DeltaY) / LengthSquared, 0.0f, 1.0f)
                : 0.0f;
            const float OffsetX = Point.X - (Start.X + DeltaX * Alpha);
            const float OffsetY = Point.Y - (Start.Y + DeltaY * Alpha);
            if (OffsetX * OffsetX + OffsetY * OffsetY <= RadiusSquared)
            {
                return true;
            }
        }
        return false;
    };

    FRandomStream RandomStream(5812403);
    const float HalfWidth = GroundScale.X * 100.0f;
    const float HalfDepth = GroundScale.Y * 100.0f;
    constexpr float CellSpacing = 185.0f;
    constexpr float GroundSurfaceZ = 3.0f;

    for (float GridX = -HalfWidth + 140.0f; GridX < HalfWidth - 140.0f; GridX += CellSpacing)
    {
        for (float GridY = -HalfDepth + 140.0f; GridY < HalfDepth - 140.0f; GridY += CellSpacing)
        {
            const float X = GridX + RandomStream.FRandRange(-52.0f, 52.0f);
            const float Y = GridY + RandomStream.FRandRange(-52.0f, 52.0f);
            const FVector2D GroundPoint(X, Y);

            if (IsNearPath(GroundPoint, Paths[0], 410.0f)
                || IsNearPath(GroundPoint, Paths[1], 255.0f)
                || IsNearPath(GroundPoint, Paths[2], 205.0f)
                || IsNearPath(GroundPoint, Paths[3], 150.0f))
            {
                continue;
            }

            const bool bInVillage = FMath::Abs(X) < 960.0f && FMath::Abs(Y) < 890.0f;
            const bool bInCastle = FMath::Abs(X - 1050.0f) < 760.0f && FMath::Abs(Y - 1150.0f) < 640.0f;
            const bool bInGreenField = FMath::Abs(X + 1980.0f) < 600.0f && FMath::Abs(Y) < 430.0f;
            const bool bInWheatField = FMath::Abs(X + 1690.0f) < 610.0f && FMath::Abs(Y + 1260.0f) < 390.0f;
            const bool bInOrchard = FMath::Abs(X + 760.0f) < 480.0f && FMath::Abs(Y + 1430.0f) < 370.0f;
            const bool bOnLowHill =
                (FMath::Abs(X + 2180.0f) < 460.0f && FMath::Abs(Y - 1420.0f) < 390.0f)
                || (FMath::Abs(X - 2180.0f) < 460.0f && FMath::Abs(Y - 1450.0f) < 410.0f)
                || (FMath::Abs(X + 2200.0f) < 420.0f && FMath::Abs(Y + 1470.0f) < 360.0f)
                || (FMath::Abs(X - 2250.0f) < 440.0f && FMath::Abs(Y + 1510.0f) < 390.0f);
            const bool bNearLandmark =
                (FMath::Abs(X + 2050.0f) < 260.0f && FMath::Abs(Y - 720.0f) < 250.0f)
                || (FMath::Abs(X - 1980.0f) < 360.0f && FMath::Abs(Y - 780.0f) < 330.0f)
                || (FMath::Abs(X + 2040.0f) < 230.0f && FMath::Abs(Y - 1260.0f) < 210.0f);

            if (bInVillage || bInCastle || bInGreenField || bInWheatField || bInOrchard || bOnLowHill || bNearLandmark)
            {
                continue;
            }

            if (RuntimeCC0GroundCoverMeshes.Num() > 0)
            {
                const int32 ClumpCount = 2 + RandomStream.RandRange(0, 1);
                for (int32 ClumpIndex = 0; ClumpIndex < ClumpCount; ++ClumpIndex)
                {
                    const int32 MeshIndex = RandomStream.RandRange(0, RuntimeCC0GroundCoverMeshes.Num() - 1);
                    UStaticMesh* GrassMesh = RuntimeCC0GroundCoverMeshes[MeshIndex].Get();
                    if (!GrassMesh)
                    {
                        continue;
                    }

                    const FName BatchName(*FString::Printf(TEXT("CC0_GroundCover_%s"), *GrassMesh->GetName()));
                    AddFoliageInstance(
                        GrassMesh,
                        BatchName,
                        FVector(X + RandomStream.FRandRange(-45.0f, 45.0f),
                            Y + RandomStream.FRandRange(-45.0f, 45.0f), GroundSurfaceZ),
                        RandomStream.FRandRange(30.0f, 46.0f),
                        RandomStream.FRandRange(0.95f, 1.45f),
                        FRotator(0.0f, RandomStream.FRandRange(0.0f, 360.0f), 0.0f));
                }

                if (RuntimeCC0WildflowerMeshes.Num() > 0 && RandomStream.FRand() < 0.055f)
                {
                    const int32 FlowerIndex = RandomStream.RandRange(0, RuntimeCC0WildflowerMeshes.Num() - 1);
                    if (UStaticMesh* FlowerMesh = RuntimeCC0WildflowerMeshes[FlowerIndex].Get())
                    {
                        const FName BatchName(*FString::Printf(TEXT("CC0_Wildflower_%s"), *FlowerMesh->GetName()));
                        AddFoliageInstance(
                            FlowerMesh,
                            BatchName,
                            FVector(X, Y, GroundSurfaceZ),
                            RandomStream.FRandRange(34.0f, 48.0f),
                            RandomStream.FRandRange(0.95f, 1.20f),
                            FRotator(0.0f, RandomStream.FRandRange(0.0f, 360.0f), 0.0f));
                    }
                }
                continue;
            }

            const int32 BladeCount = 4 + RandomStream.RandRange(0, 2);
            for (int32 BladeIndex = 0; BladeIndex < BladeCount; ++BladeIndex)
            {
                const float AngleDegrees = RandomStream.FRandRange(0.0f, 360.0f);
                const float AngleRadians = FMath::DegreesToRadians(AngleDegrees);
                const float TuftRadius = RandomStream.FRandRange(8.0f, 47.0f);
                const float Height = RandomStream.FRandRange(32.0f, 58.0f);
                const float Lean = RandomStream.FRandRange(8.0f, 24.0f);
                const FVector BladeLocation(
                    X + FMath::Cos(AngleRadians) * TuftRadius,
                    Y + FMath::Sin(AngleRadians) * TuftRadius,
                    GroundSurfaceZ + Height * 0.5f);
                const FRotator BladeRotation(
                    RandomStream.FRandRange(-Lean, Lean),
                    AngleDegrees,
                    RandomStream.FRandRange(-Lean, Lean));
                const int32 ColorIndex = RandomStream.RandRange(0, 3);

                AddInstancedPrimitive(
                    RuntimeConeMesh,
                    BladeInstanceNames[ColorIndex],
                    BladeLocation,
                    FVector(0.018f, 0.018f, Height / 100.0f),
                    BladeColors[ColorIndex],
                    false,
                    BladeRotation,
                    false);
            }

            if (RandomStream.FRand() < 0.055f)
            {
                const int32 FlowerColorIndex = RandomStream.RandRange(0, 2);
                AddInstancedPrimitive(
                    RuntimeSphereMesh,
                    FlowerInstanceNames[FlowerColorIndex],
                    FVector(X, Y, RandomStream.FRandRange(19.0f, 31.0f)),
                    FVector(0.045f, 0.045f, 0.055f),
                    FlowerColors[FlowerColorIndex],
                    false,
                    FRotator::ZeroRotator,
                    false);
            }
        }
    }
}

void AAetherDevelopmentWorldActor::BuildRiverAndRoads()
{
    const TArray<FVector> RiverPoints = {
        FVector(-2660.0f, 1720.0f, 0.0f),
        FVector(-2180.0f, 1410.0f, 0.0f),
        FVector(-1640.0f, 1130.0f, 0.0f),
        FVector(-1030.0f, 870.0f, 0.0f),
        FVector(-470.0f, 660.0f, 0.0f),
        FVector(120.0f, 490.0f, 0.0f),
        FVector(700.0f, 230.0f, 0.0f),
        FVector(1330.0f, -90.0f, 0.0f),
        FVector(1950.0f, -400.0f, 0.0f),
        FVector(2660.0f, -750.0f, 0.0f)
    };

    AddRibbon(RiverPoints, 600.0f, 1.0f, 14.0f,
        FLinearColor(0.74f, 0.66f, 0.47f, 1.0f), TEXT("RiverSandyBanks"));
    AddRibbon(RiverPoints, 470.0f, 8.0f, 12.0f, DeepWaterColor, TEXT("RiverDeepChannel"));
    AddRibbon(RiverPoints, 340.0f, 15.0f, 8.0f, WaterColor, TEXT("RiverSurface"));

    TArray<FVector> WaterGlint;
    WaterGlint.Reserve(RiverPoints.Num());
    for (const FVector& Point : RiverPoints)
    {
        WaterGlint.Add(Point + FVector(-12.0f, 46.0f, 0.0f));
    }
    AddRibbon(WaterGlint, 26.0f, 20.0f, 3.0f,
        FLinearColor(0.50f, 0.86f, 0.91f, 1.0f), TEXT("RiverSunGlint"), false);

    const TArray<FVector> MainRoad = {
        FVector(0.0f, -1740.0f, 0.0f),
        FVector(-40.0f, -1190.0f, 0.0f),
        FVector(-130.0f, -700.0f, 0.0f),
        FVector(-80.0f, -280.0f, 0.0f),
        FVector(0.0f, 0.0f, 0.0f),
        FVector(-60.0f, 280.0f, 0.0f),
        FVector(120.0f, 490.0f, 0.0f),
        FVector(350.0f, 720.0f, 0.0f),
        FVector(420.0f, 540.0f, 0.0f),
        FVector(700.0f, 520.0f, 0.0f),
        FVector(970.0f, 580.0f, 0.0f),
        FVector(1050.0f, 695.0f, 0.0f),
        FVector(1050.0f, 1080.0f, 0.0f)
    };
    const FLinearColor RoadEdge(0.39f, 0.27f, 0.17f, 1.0f);
    const FLinearColor RoadCenter(0.66f, 0.51f, 0.32f, 1.0f);
    AddRibbon(MainRoad, 355.0f, 7.0f, 14.0f, RoadEdge, TEXT("MainRoadEdge"));
    AddRibbon(MainRoad, 268.0f, 15.0f, 9.0f, RoadCenter, TEXT("MainRoadSurface"));

    const TArray<FVector> FarmRoad = {
        FVector(-120.0f, -270.0f, 0.0f),
        FVector(-430.0f, -365.0f, 0.0f),
        FVector(-780.0f, -425.0f, 0.0f),
        FVector(-1160.0f, -500.0f, 0.0f),
        FVector(-1560.0f, -565.0f, 0.0f),
        FVector(-1930.0f, -780.0f, 0.0f)
    };
    AddRibbon(FarmRoad, 265.0f, 7.0f, 14.0f, RoadEdge, TEXT("FarmRoadEdge"));
    AddRibbon(FarmRoad, 194.0f, 15.0f, 9.0f, RoadCenter, TEXT("FarmRoadSurface"));

    const TArray<FVector> OrchardPath = {
        FVector(-450.0f, 100.0f, 0.0f),
        FVector(-760.0f, 380.0f, 0.0f),
        FVector(-1160.0f, 640.0f, 0.0f),
        FVector(-1530.0f, 810.0f, 0.0f)
    };
    AddRibbon(OrchardPath, 168.0f, 8.0f, 12.0f,
        FLinearColor(0.51f, 0.37f, 0.23f, 1.0f), TEXT("OrchardPathEdge"));
    AddRibbon(OrchardPath, 118.0f, 15.0f, 8.0f,
        FLinearColor(0.72f, 0.57f, 0.37f, 1.0f), TEXT("OrchardPath"));
}

void AAetherDevelopmentWorldActor::BuildBridge()
{
    const FVector BridgeCenter(120.0f, 490.0f, 0.0f);
    const float Yaw = 48.0f;
    const float Length = 690.0f;
    const float DeckWidth = 220.0f;
    const FRotator BridgeRotation(0.0f, Yaw, 0.0f);
    const float YawRadians = FMath::DegreesToRadians(Yaw);
    const FVector Side(-FMath::Sin(YawRadians), FMath::Cos(YawRadians), 0.0f);

    AddPrimitive(RuntimeCubeMesh, TEXT("BridgeStoneFoundation"),
        BridgeCenter + FVector(0.0f, 0.0f, 22.0f),
        FVector(Length / 100.0f, DeckWidth / 100.0f, 0.22f),
        FLinearColor(0.43f, 0.45f, 0.44f, 1.0f), true, BridgeRotation);
    AddPrimitive(RuntimeCubeMesh, TEXT("BridgePavedDeck"),
        BridgeCenter + FVector(0.0f, 0.0f, 35.0f),
        FVector(Length / 100.0f, (DeckWidth - 28.0f) / 100.0f, 0.08f),
        FLinearColor(0.77f, 0.75f, 0.67f, 1.0f), true, BridgeRotation);

    for (int32 SideIndex = -1; SideIndex <= 1; SideIndex += 2)
    {
        AddInstancedPrimitive(RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("BridgeParapet_%d"), SideIndex)),
            BridgeCenter + Side * (DeckWidth * 0.43f * SideIndex) + FVector(0.0f, 0.0f, 70.0f),
            FVector(Length / 100.0f, 0.22f, 0.64f),
            StoneColor, true, BridgeRotation);
    }

    const int32 PostCount = 6;
    for (int32 Index = 0; Index <= PostCount; ++Index)
    {
        const float Along = -Length * 0.5f + Length * static_cast<float>(Index) / static_cast<float>(PostCount);
        const FVector PostBase = OffsetAlongYaw(BridgeCenter, Along, Yaw);
        for (int32 SideIndex = -1; SideIndex <= 1; SideIndex += 2)
        {
        AddInstancedPrimitive(RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("BridgePost_%d_%d"), Index, SideIndex)),
                PostBase + Side * (DeckWidth * 0.43f * SideIndex) + FVector(0.0f, 0.0f, 62.0f),
                FVector(0.28f, 0.28f, 0.75f),
                WarmStoneColor, false, BridgeRotation);
        }
    }

    AddInstancedPrimitive(RuntimeCylinderMesh, TEXT("BridgePier_Center"),
        BridgeCenter + FVector(0.0f, 0.0f, -14.0f), FVector(0.75f, 0.75f, 1.1f), StoneColor);
    AddInstancedPrimitive(RuntimeCylinderMesh, TEXT("BridgePier_East"),
        OffsetAlongYaw(BridgeCenter, Length * 0.32f, Yaw) + FVector(0.0f, 0.0f, -6.0f),
        FVector(0.64f, 0.64f, 0.82f), StoneColor);
}

void AAetherDevelopmentWorldActor::BuildVillage()
{
    AddPrimitive(RuntimeCylinderMesh, TEXT("VillagePlazaStoneBase"),
        FVector(0.0f, 0.0f, 10.0f), FVector(8.6f, 7.0f, 0.22f),
        FLinearColor(0.48f, 0.48f, 0.43f, 1.0f));
    AddPrimitive(RuntimeCylinderMesh, TEXT("VillagePlazaPaving"),
        FVector(0.0f, 0.0f, 22.0f), FVector(8.25f, 6.65f, 0.08f),
        FLinearColor(0.78f, 0.75f, 0.64f, 1.0f));

    const FLinearColor FountainStone(0.66f, 0.67f, 0.62f, 1.0f);
    AddPrimitive(RuntimeCylinderMesh, TEXT("VillageFountainBasin"),
        FVector(-170.0f, 142.0f, 48.0f), FVector(1.62f, 1.62f, 0.34f), FountainStone);
    AddPrimitive(RuntimeCylinderMesh, TEXT("VillageFountainWater"),
        FVector(-170.0f, 142.0f, 67.0f), FVector(1.38f, 1.38f, 0.12f), WaterColor);
    AddPrimitive(RuntimeCylinderMesh, TEXT("VillageFountainColumn"),
        FVector(-170.0f, 142.0f, 94.0f), FVector(0.34f, 0.34f, 0.54f), FountainStone);
    AddPrimitive(RuntimeSphereMesh, TEXT("VillageFountainFinial"),
        FVector(-170.0f, 142.0f, 126.0f), FVector(0.35f, 0.35f, 0.38f),
        FLinearColor(0.50f, 0.80f, 0.90f, 1.0f));

    BuildHouse(FVector(-680.0f, -635.0f, 0.0f), 0.98f,
        FLinearColor(0.60f, 0.18f, 0.10f, 1.0f), TEXT("VillageInn"), -12.0f,
        FLinearColor(0.84f, 0.72f, 0.52f, 1.0f));
    BuildHouse(FVector(-970.0f, 75.0f, 0.0f), 0.88f,
        FLinearColor(0.47f, 0.17f, 0.13f, 1.0f), TEXT("VillageForge"), 18.0f,
        FLinearColor(0.73f, 0.66f, 0.53f, 1.0f));
    BuildHouse(FVector(760.0f, -690.0f, 0.0f), 0.92f,
        FLinearColor(0.67f, 0.30f, 0.10f, 1.0f), TEXT("VillageTavern"), 24.0f,
        FLinearColor(0.88f, 0.77f, 0.59f, 1.0f));
    BuildHouse(FVector(860.0f, 20.0f, 0.0f), 0.77f,
        FLinearColor(0.12f, 0.31f, 0.66f, 1.0f), TEXT("VillageCottageNorth"), -8.0f,
        FLinearColor(0.88f, 0.82f, 0.66f, 1.0f));
    BuildHouse(FVector(-440.0f, -1110.0f, 0.0f), 0.82f,
        FLinearColor(0.63f, 0.30f, 0.11f, 1.0f), TEXT("VillageCottageSouth"), 8.0f,
        FLinearColor(0.77f, 0.72f, 0.57f, 1.0f));
    BuildHouse(FVector(-1880.0f, -640.0f, 0.0f), 0.86f,
        FLinearColor(0.58f, 0.20f, 0.12f, 1.0f), TEXT("Farmhouse"), -18.0f,
        FLinearColor(0.84f, 0.75f, 0.58f, 1.0f));

    BuildMarketStall(FVector(-320.0f, -320.0f, 0.0f),
        FLinearColor(0.76f, 0.17f, 0.10f, 1.0f), TEXT("MarketStallRed"));
    BuildMarketStall(FVector(310.0f, -330.0f, 0.0f),
        FLinearColor(0.18f, 0.38f, 0.70f, 1.0f), TEXT("MarketStallBlue"));
    BuildMarketStall(FVector(-360.0f, 360.0f, 0.0f),
        FLinearColor(0.72f, 0.48f, 0.10f, 1.0f), TEXT("MarketStallGold"));

    BuildLantern(FVector(-520.0f, -40.0f, 0.0f), TEXT("VillageLanternWest"));
    BuildLantern(FVector(510.0f, -40.0f, 0.0f), TEXT("VillageLanternEast"));
    BuildLantern(FVector(-330.0f, 680.0f, 0.0f), TEXT("RoadLanternOne"));
    BuildLantern(FVector(400.0f, 700.0f, 0.0f), TEXT("RoadLanternTwo"));

    BuildTree(FVector(-1130.0f, -920.0f, 0.0f), 0.72f, 0, TEXT("VillageTreeWest"));
    BuildTree(FVector(1110.0f, -550.0f, 0.0f), 0.80f, 2, TEXT("VillageTreeEast"));
    BuildTree(FVector(1240.0f, 240.0f, 0.0f), 0.78f, 0, TEXT("VillageTreeNorth"));
    BuildTree(FVector(-870.0f, 550.0f, 0.0f), 0.86f, 1, TEXT("VillagePineNorthWest"));

    BuildFence(FVector(-1150.0f, -1080.0f, 0.0f), FVector(-620.0f, -1080.0f, 0.0f),
        78.0f, TEXT("VillageGardenFenceSouth"), TimberColor);
}

void AAetherDevelopmentWorldActor::BuildCastle()
{
    const FVector Center(1050.0f, 1150.0f, 0.0f);
    const FLinearColor CastleWall(0.82f, 0.82f, 0.74f, 1.0f);
    const FLinearColor CastleStone(0.58f, 0.59f, 0.55f, 1.0f);

    AddPrimitive(RuntimeSphereMesh, TEXT("CastleHill"),
        Center + FVector(0.0f, 0.0f, 36.0f), FVector(14.0f, 11.5f, 1.18f),
        FLinearColor(0.36f, 0.54f, 0.23f, 1.0f));
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleTerraceFoundation"),
        Center + FVector(0.0f, 0.0f, 82.0f), FVector(13.0f, 10.6f, 0.54f),
        CastleStone, true);
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleCourtyardGrass"),
        Center + FVector(0.0f, 0.0f, 111.0f), FVector(12.2f, 9.9f, 0.10f),
        FLinearColor(0.35f, 0.54f, 0.22f, 1.0f));

    const float WallCenterZ = 260.0f;
    const float WallHeightScale = 2.85f;
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleNorthCurtainWall"),
        Center + FVector(0.0f, 455.0f, WallCenterZ), FVector(11.8f, 0.48f, WallHeightScale),
        CastleWall, true);
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleWestCurtainWall"),
        Center + FVector(-565.0f, 0.0f, WallCenterZ), FVector(0.48f, 9.1f, WallHeightScale),
        CastleWall, true);
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleEastCurtainWall"),
        Center + FVector(565.0f, 0.0f, WallCenterZ), FVector(0.48f, 9.1f, WallHeightScale),
        CastleWall, true);
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleSouthWallWest"),
        Center + FVector(-372.0f, -455.0f, WallCenterZ), FVector(4.2f, 0.48f, WallHeightScale),
        CastleWall, true);
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleSouthWallEast"),
        Center + FVector(372.0f, -455.0f, WallCenterZ), FVector(4.2f, 0.48f, WallHeightScale),
        CastleWall, true);

    BuildTower(Center + FVector(-550.0f, -445.0f, 115.0f), 470.0f, 104.0f,
        BlueRoofColor, TEXT("CastleTowerSouthWest"));
    BuildTower(Center + FVector(550.0f, -445.0f, 115.0f), 510.0f, 108.0f,
        BlueRoofColor, TEXT("CastleTowerSouthEast"));
    BuildTower(Center + FVector(-550.0f, 445.0f, 115.0f), 600.0f, 118.0f,
        BlueRoofColor, TEXT("CastleTowerNorthWest"));
    BuildTower(Center + FVector(550.0f, 445.0f, 115.0f), 575.0f, 114.0f,
        BlueRoofColor, TEXT("CastleTowerNorthEast"));
    BuildTower(Center + FVector(0.0f, 300.0f, 115.0f), 690.0f, 112.0f,
        FLinearColor(0.08f, 0.24f, 0.62f, 1.0f), TEXT("CastleHighKeepTower"));

    BuildTower(Center + FVector(-195.0f, -462.0f, 115.0f), 370.0f, 78.0f,
        FLinearColor(0.16f, 0.38f, 0.76f, 1.0f), TEXT("CastleGateTowerWest"));
    BuildTower(Center + FVector(195.0f, -462.0f, 115.0f), 370.0f, 78.0f,
        FLinearColor(0.16f, 0.38f, 0.76f, 1.0f), TEXT("CastleGateTowerEast"));

    AddPrimitive(RuntimeCubeMesh, TEXT("CastleGateArch"),
        Center + FVector(0.0f, -458.0f, 438.0f), FVector(4.25f, 0.62f, 0.56f),
        CastleWall, true);
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleGateDarkPassage"),
        Center + FVector(0.0f, -468.0f, 174.0f), FVector(2.40f, 0.12f, 3.05f),
        FLinearColor(0.12f, 0.16f, 0.20f, 1.0f));

    BuildHouse(Center + FVector(0.0f, 70.0f, 115.0f), 1.55f,
        BlueRoofColor, TEXT("CastleGreatHall"), 0.0f,
        FLinearColor(0.88f, 0.88f, 0.80f, 1.0f));
    BuildHouse(Center + FVector(-330.0f, -110.0f, 115.0f), 0.86f,
        FLinearColor(0.16f, 0.34f, 0.71f, 1.0f), TEXT("CastleWestWing"), 0.0f,
        FLinearColor(0.83f, 0.83f, 0.76f, 1.0f));
    BuildHouse(Center + FVector(330.0f, -110.0f, 115.0f), 0.86f,
        FLinearColor(0.16f, 0.34f, 0.71f, 1.0f), TEXT("CastleEastWing"), 0.0f,
        FLinearColor(0.83f, 0.83f, 0.76f, 1.0f));

    AddPrimitive(RuntimeCylinderMesh, TEXT("CastleCourtyardFountainBase"),
        Center + FVector(-245.0f, -180.0f, 142.0f), FVector(1.10f, 1.10f, 0.25f),
        CastleStone);
    AddPrimitive(RuntimeCylinderMesh, TEXT("CastleCourtyardFountainWater"),
        Center + FVector(-245.0f, -180.0f, 157.0f), FVector(0.86f, 0.86f, 0.09f), WaterColor);

    AddPrimitive(RuntimeCubeMesh, TEXT("CastleGardenHedgeNorth"),
        Center + FVector(-300.0f, -250.0f, 160.0f), FVector(2.4f, 0.24f, 0.75f),
        FLinearColor(0.13f, 0.38f, 0.14f, 1.0f));
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleGardenHedgeWest"),
        Center + FVector(-230.0f, -335.0f, 160.0f), FVector(0.24f, 1.5f, 0.75f),
        FLinearColor(0.16f, 0.43f, 0.16f, 1.0f));
    AddPrimitive(RuntimeCubeMesh, TEXT("CastleGardenHedgeEast"),
        Center + FVector(230.0f, -335.0f, 160.0f), FVector(0.24f, 1.5f, 0.75f),
        FLinearColor(0.16f, 0.43f, 0.16f, 1.0f));

    for (int32 Index = 0; Index < 7; ++Index)
    {
        const float X = -450.0f + 150.0f * static_cast<float>(Index);
        AddInstancedPrimitive(RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("CastleNorthCrenel_%d"), Index)),
            Center + FVector(X, 455.0f, 430.0f), FVector(0.52f, 0.62f, 0.52f), CastleWall);
    }
    for (int32 Index = 0; Index < 5; ++Index)
    {
        const float Y = -300.0f + 150.0f * static_cast<float>(Index);
        AddInstancedPrimitive(RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("CastleWestCrenel_%d"), Index)),
            Center + FVector(-565.0f, Y, 430.0f), FVector(0.62f, 0.52f, 0.52f), CastleWall);
        AddInstancedPrimitive(RuntimeCubeMesh,
            FName(*FString::Printf(TEXT("CastleEastCrenel_%d"), Index)),
            Center + FVector(565.0f, Y, 430.0f), FVector(0.62f, 0.52f, 0.52f), CastleWall);
    }
}

void AAetherDevelopmentWorldActor::BuildFarms()
{
    BuildFarmPlot(FVector(-1980.0f, 0.0f, 0.0f), 980.0f, 650.0f, 14.0f,
        FLinearColor(0.36f, 0.28f, 0.16f, 1.0f),
        FLinearColor(0.28f, 0.52f, 0.16f, 1.0f), TEXT("GreenVegetableField"));
    BuildFarmPlot(FVector(-1690.0f, -1260.0f, 0.0f), 990.0f, 580.0f, -10.0f,
        FLinearColor(0.40f, 0.31f, 0.18f, 1.0f),
        FLinearColor(0.77f, 0.59f, 0.20f, 1.0f), TEXT("GoldenWheatField"));
    BuildFarmPlot(FVector(-760.0f, -1430.0f, 0.0f), 760.0f, 520.0f, 18.0f,
        FLinearColor(0.32f, 0.27f, 0.16f, 1.0f),
        FLinearColor(0.31f, 0.58f, 0.20f, 1.0f), TEXT("OrchardRows"));

    AddPrimitive(RuntimeCylinderMesh, TEXT("FarmSilo"),
        FVector(-2210.0f, -330.0f, 126.0f), FVector(1.55f, 1.55f, 2.42f),
        FLinearColor(0.82f, 0.73f, 0.55f, 1.0f), true);
    AddPrimitive(RuntimeConeMesh, TEXT("FarmSiloRoof"),
        FVector(-2210.0f, -330.0f, 260.0f), FVector(1.70f, 1.70f, 1.12f),
        FLinearColor(0.53f, 0.20f, 0.13f, 1.0f));

    BuildFence(FVector(-2290.0f, -920.0f, 0.0f), FVector(-1990.0f, -920.0f, 0.0f),
        105.0f, TEXT("FarmyardFenceSouth"), TimberColor);
    BuildFence(FVector(-2290.0f, -920.0f, 0.0f), FVector(-2290.0f, -520.0f, 0.0f),
        105.0f, TEXT("FarmyardFenceWest"), TimberColor);
}

void AAetherDevelopmentWorldActor::BuildForest()
{
    const FVector TreeLocations[] = {
        FVector(-2450.0f, 1780.0f, 0.0f), FVector(-2160.0f, 1710.0f, 0.0f),
        FVector(-1900.0f, 1840.0f, 0.0f), FVector(-1640.0f, 1660.0f, 0.0f),
        FVector(-1350.0f, 1830.0f, 0.0f), FVector(-1080.0f, 1710.0f, 0.0f),
        FVector(-790.0f, 1850.0f, 0.0f), FVector(-510.0f, 1640.0f, 0.0f),
        FVector(-240.0f, 1850.0f, 0.0f), FVector(80.0f, 1730.0f, 0.0f),
        FVector(380.0f, 1840.0f, 0.0f), FVector(670.0f, 1640.0f, 0.0f),
        FVector(1840.0f, 1840.0f, 0.0f), FVector(2110.0f, 1710.0f, 0.0f),
        FVector(2390.0f, 1790.0f, 0.0f), FVector(2220.0f, 1450.0f, 0.0f),
        FVector(2380.0f, 1120.0f, 0.0f), FVector(2220.0f, 790.0f, 0.0f),
        FVector(2410.0f, 470.0f, 0.0f), FVector(2230.0f, 120.0f, 0.0f),
        FVector(2400.0f, -210.0f, 0.0f), FVector(2220.0f, -530.0f, 0.0f),
        FVector(2400.0f, -880.0f, 0.0f), FVector(2210.0f, -1210.0f, 0.0f),
        FVector(2390.0f, -1570.0f, 0.0f), FVector(2100.0f, -1740.0f, 0.0f),
        FVector(-2440.0f, -1020.0f, 0.0f), FVector(-2280.0f, -1320.0f, 0.0f),
        FVector(-2020.0f, -1600.0f, 0.0f), FVector(-1730.0f, -1770.0f, 0.0f),
        FVector(-1400.0f, -1740.0f, 0.0f), FVector(1730.0f, 1530.0f, 0.0f),
        FVector(1940.0f, 1260.0f, 0.0f), FVector(2110.0f, 930.0f, 0.0f),
        FVector(-2250.0f, 670.0f, 0.0f), FVector(-2130.0f, 360.0f, 0.0f),
        FVector(-2360.0f, 40.0f, 0.0f), FVector(1850.0f, -1470.0f, 0.0f)
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(TreeLocations); ++Index)
    {
        const float Scale = 0.82f + static_cast<float>(Index % 4) * 0.10f;
        const int32 Variant = (Index * 7 + Index / 3) % 5;
        BuildTree(TreeLocations[Index], Scale, Variant,
            FName(*FString::Printf(TEXT("RegionTree_%02d"), Index)));
    }

    BuildRockCluster(FVector(-2380.0f, 1110.0f, 0.0f), 1.15f, TEXT("MountainRocksWest"));
    BuildRockCluster(FVector(2300.0f, 1510.0f, 0.0f), 1.25f, TEXT("MountainRocksEast"));
    BuildRockCluster(FVector(2160.0f, -1510.0f, 0.0f), 1.10f, TEXT("MountainRocksSouthEast"));
}

void AAetherDevelopmentWorldActor::BuildImportedVegetation()
{
    const bool bHasCC0Trees = RuntimeCC0BroadleafTreeMeshes.Num() > 0 || RuntimeCC0PineTreeMeshes.Num() > 0;
    const bool bHasCC0Bushes = RuntimeCC0BushMeshes.Num() > 0;
    const bool bHasFabTrees = RuntimeFabTreeBroadleafMesh || RuntimeFabTreeSmallMesh || RuntimeFabPineMesh;
    const bool bHasFabBushes = RuntimeFabBushLargeMesh || RuntimeFabBushMediumMesh || RuntimeFabBushFlowerMesh;
    const bool bHasTrees = bHasCC0Trees || bHasFabTrees;
    const bool bHasBushes = bHasCC0Bushes || bHasFabBushes;
    if (!bHasTrees && !bHasBushes)
    {
        return;
    }

    const TArray<FVector> RiverPath = {
        FVector(-2660.0f, 1720.0f, 0.0f), FVector(-2180.0f, 1410.0f, 0.0f),
        FVector(-1640.0f, 1130.0f, 0.0f), FVector(-1030.0f, 870.0f, 0.0f),
        FVector(-470.0f, 660.0f, 0.0f), FVector(120.0f, 490.0f, 0.0f),
        FVector(700.0f, 230.0f, 0.0f), FVector(1330.0f, -90.0f, 0.0f),
        FVector(1950.0f, -400.0f, 0.0f), FVector(2660.0f, -750.0f, 0.0f)
    };
    const TArray<FVector> MainRoad = {
        FVector(0.0f, -1740.0f, 0.0f), FVector(-40.0f, -1190.0f, 0.0f),
        FVector(-130.0f, -700.0f, 0.0f), FVector(-80.0f, -280.0f, 0.0f),
        FVector(0.0f, 0.0f, 0.0f), FVector(-60.0f, 280.0f, 0.0f),
        FVector(120.0f, 490.0f, 0.0f), FVector(350.0f, 720.0f, 0.0f),
        FVector(420.0f, 540.0f, 0.0f), FVector(700.0f, 520.0f, 0.0f),
        FVector(970.0f, 580.0f, 0.0f), FVector(1050.0f, 695.0f, 0.0f),
        FVector(1050.0f, 1080.0f, 0.0f)
    };
    const TArray<FVector> FarmRoad = {
        FVector(-120.0f, -270.0f, 0.0f), FVector(-430.0f, -365.0f, 0.0f),
        FVector(-780.0f, -425.0f, 0.0f), FVector(-1160.0f, -500.0f, 0.0f),
        FVector(-1560.0f, -565.0f, 0.0f), FVector(-1930.0f, -780.0f, 0.0f)
    };
    const TArray<FVector> OrchardPath = {
        FVector(-450.0f, 100.0f, 0.0f), FVector(-760.0f, 380.0f, 0.0f),
        FVector(-1160.0f, 640.0f, 0.0f), FVector(-1530.0f, 810.0f, 0.0f)
    };
    const auto IsNearPath = [](const FVector& Point, const TArray<FVector>& Path, float Radius)
    {
        const float RadiusSquared = Radius * Radius;
        for (int32 SegmentIndex = 0; SegmentIndex < Path.Num() - 1; ++SegmentIndex)
        {
            const FVector& Start = Path[SegmentIndex];
            const FVector& End = Path[SegmentIndex + 1];
            const FVector2D Delta(End.X - Start.X, End.Y - Start.Y);
            const FVector2D ToPoint(Point.X - Start.X, Point.Y - Start.Y);
            const float LengthSquared = Delta.SizeSquared();
            const float Alpha = LengthSquared > SMALL_NUMBER
                ? FMath::Clamp(FVector2D::DotProduct(ToPoint, Delta) / LengthSquared, 0.0f, 1.0f)
                : 0.0f;
            const FVector2D Offset = ToPoint - Delta * Alpha;
            if (Offset.SizeSquared() <= RadiusSquared)
            {
                return true;
            }
        }
        return false;
    };

    FRandomStream Random(390217);

    if (bHasTrees)
    {
        for (int32 Index = 0; Index < 260; ++Index)
        {
            const int32 Side = Random.RandRange(0, 3);
            FVector Location;
            if (Side == 0)
            {
                Location = FVector(Random.FRandRange(-3500.0f, 3500.0f), Random.FRandRange(2320.0f, 2940.0f), 0.0f);
            }
            else if (Side == 1)
            {
                Location = FVector(Random.FRandRange(-3500.0f, 3500.0f), Random.FRandRange(-2940.0f, -2320.0f), 0.0f);
            }
            else if (Side == 2)
            {
                Location = FVector(Random.FRandRange(-3860.0f, -3120.0f), Random.FRandRange(-2050.0f, 2050.0f), 0.0f);
            }
            else
            {
                Location = FVector(Random.FRandRange(3120.0f, 3860.0f), Random.FRandRange(-2050.0f, 2050.0f), 0.0f);
            }

            if (IsNearPath(Location, RiverPath, 370.0f)
                || IsNearPath(Location, MainRoad, 390.0f)
                || IsNearPath(Location, FarmRoad, 250.0f)
                || IsNearPath(Location, OrchardPath, 225.0f)
                || (RuntimeFabMansionMesh
                    && FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(3050.0f, 1380.0f)) < 1000.0f))
            {
                continue;
            }

            const bool bWantsPine = Random.FRand() < 0.23f;
            const bool bWantsSmall = !bWantsPine && Random.FRand() < 0.38f;
            UStaticMesh* TreeMesh = nullptr;
            bool bSelectedCC0 = false;

            if (bHasCC0Trees)
            {
                const TArray<TObjectPtr<UStaticMesh>>& PreferredPool = bWantsPine && RuntimeCC0PineTreeMeshes.Num() > 0
                    ? RuntimeCC0PineTreeMeshes
                    : RuntimeCC0BroadleafTreeMeshes;
                if (PreferredPool.Num() > 0)
                {
                    const int32 MeshIndex = Random.RandRange(0, PreferredPool.Num() - 1);
                    TreeMesh = PreferredPool[MeshIndex].Get();
                }
                if (!TreeMesh && RuntimeCC0BroadleafTreeMeshes.Num() > 0)
                {
                    TreeMesh = RuntimeCC0BroadleafTreeMeshes[Random.RandRange(0, RuntimeCC0BroadleafTreeMeshes.Num() - 1)].Get();
                }
                if (!TreeMesh && RuntimeCC0PineTreeMeshes.Num() > 0)
                {
                    TreeMesh = RuntimeCC0PineTreeMeshes[Random.RandRange(0, RuntimeCC0PineTreeMeshes.Num() - 1)].Get();
                }
                bSelectedCC0 = TreeMesh != nullptr;
            }

            if (!TreeMesh)
            {
                if (bWantsPine && RuntimeFabPineMesh)
                {
                    TreeMesh = RuntimeFabPineMesh.Get();
                }
                else if (bWantsSmall && RuntimeFabTreeSmallMesh)
                {
                    TreeMesh = RuntimeFabTreeSmallMesh.Get();
                }
                else
                {
                    TreeMesh = RuntimeFabTreeBroadleafMesh
                        ? RuntimeFabTreeBroadleafMesh.Get()
                        : (RuntimeFabTreeSmallMesh ? RuntimeFabTreeSmallMesh.Get() : RuntimeFabPineMesh.Get());
                }
            }
            if (!TreeMesh)
            {
                continue;
            }

            const FName BatchName = bSelectedCC0
                ? FName(*FString::Printf(TEXT("CC0_Tree_%s"), *TreeMesh->GetName()))
                : FName(TreeMesh == RuntimeFabPineMesh.Get() ? TEXT("FabPineInstances")
                    : (TreeMesh == RuntimeFabTreeSmallMesh.Get() ? TEXT("FabSmallTreeInstances") : TEXT("FabBroadleafInstances")));
            const float Height = bSelectedCC0
                ? (bWantsPine ? Random.FRandRange(580.0f, 800.0f) : Random.FRandRange(450.0f, 720.0f))
                : (TreeMesh == RuntimeFabTreeSmallMesh.Get()
                    ? Random.FRandRange(390.0f, 520.0f)
                    : Random.FRandRange(520.0f, 790.0f));
            AddFoliageInstance(
                TreeMesh,
                BatchName,
                Location,
                Height,
                Random.FRandRange(0.86f, 1.24f),
                FRotator(0.0f, Random.FRandRange(0.0f, 360.0f), 0.0f));
        }

        // Extend the woodland inward so the town and outer forest no longer feel
        // separated by large, empty grass bands. These are lower, varied trees.
        for (float GridX = -2440.0f; GridX <= 2440.0f; GridX += 460.0f)
        {
            for (float GridY = -2240.0f; GridY <= 2240.0f; GridY += 460.0f)
            {
                if (FMath::Abs(GridX) < 1380.0f && FMath::Abs(GridY) < 1120.0f)
                {
                    continue;
                }

                FVector Location(
                    GridX + Random.FRandRange(-135.0f, 135.0f),
                    GridY + Random.FRandRange(-135.0f, 135.0f),
                    0.0f);
                if (Random.FRand() > 0.58f
                    || IsNearPath(Location, RiverPath, 310.0f)
                    || IsNearPath(Location, MainRoad, 290.0f)
                    || IsNearPath(Location, FarmRoad, 210.0f)
                    || IsNearPath(Location, OrchardPath, 205.0f)
                    || (FMath::Abs(Location.X + 1980.0f) < 650.0f && FMath::Abs(Location.Y) < 470.0f)
                    || (FMath::Abs(Location.X + 1690.0f) < 650.0f && FMath::Abs(Location.Y + 1260.0f) < 430.0f)
                    || (FMath::Abs(Location.X + 760.0f) < 510.0f && FMath::Abs(Location.Y + 1430.0f) < 390.0f)
                    || FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(1050.0f, 1150.0f)) < 930.0f
                    || FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(1980.0f, 780.0f)) < 380.0f
                    || FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(-2050.0f, 720.0f)) < 330.0f
                    || (RuntimeFabMansionMesh
                        && FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(3050.0f, 1380.0f)) < 1000.0f))
                {
                    continue;
                }

                const bool bWantsPine = Random.FRand() < 0.22f;
                const bool bWantsSmall = !bWantsPine && Random.FRand() < 0.34f;
                UStaticMesh* TreeMesh = nullptr;
                bool bSelectedCC0 = false;

                if (bHasCC0Trees)
                {
                    const TArray<TObjectPtr<UStaticMesh>>& PreferredPool = bWantsPine && RuntimeCC0PineTreeMeshes.Num() > 0
                        ? RuntimeCC0PineTreeMeshes
                        : RuntimeCC0BroadleafTreeMeshes;
                    if (PreferredPool.Num() > 0)
                    {
                        TreeMesh = PreferredPool[Random.RandRange(0, PreferredPool.Num() - 1)].Get();
                    }
                    if (!TreeMesh && RuntimeCC0BroadleafTreeMeshes.Num() > 0)
                    {
                        TreeMesh = RuntimeCC0BroadleafTreeMeshes[Random.RandRange(0, RuntimeCC0BroadleafTreeMeshes.Num() - 1)].Get();
                    }
                    if (!TreeMesh && RuntimeCC0PineTreeMeshes.Num() > 0)
                    {
                        TreeMesh = RuntimeCC0PineTreeMeshes[Random.RandRange(0, RuntimeCC0PineTreeMeshes.Num() - 1)].Get();
                    }
                    bSelectedCC0 = TreeMesh != nullptr;
                }

                if (!TreeMesh)
                {
                    TreeMesh = RuntimeFabTreeBroadleafMesh.Get();
                    if (bWantsPine && RuntimeFabPineMesh)
                    {
                        TreeMesh = RuntimeFabPineMesh.Get();
                    }
                    else if (bWantsSmall && RuntimeFabTreeSmallMesh)
                    {
                        TreeMesh = RuntimeFabTreeSmallMesh.Get();
                    }
                    if (!TreeMesh)
                    {
                        TreeMesh = RuntimeFabTreeSmallMesh
                            ? RuntimeFabTreeSmallMesh.Get()
                            : RuntimeFabPineMesh.Get();
                    }
                }
                if (!TreeMesh)
                {
                    continue;
                }

                const FName BatchName = bSelectedCC0
                    ? FName(*FString::Printf(TEXT("CC0_Tree_%s"), *TreeMesh->GetName()))
                    : FName(TreeMesh == RuntimeFabPineMesh.Get() ? TEXT("FabPineInstances")
                        : (TreeMesh == RuntimeFabTreeSmallMesh.Get() ? TEXT("FabSmallTreeInstances") : TEXT("FabBroadleafInstances")));
                AddFoliageInstance(
                    TreeMesh,
                    BatchName,
                    Location,
                    bSelectedCC0
                        ? (bWantsPine ? Random.FRandRange(430.0f, 620.0f) : Random.FRandRange(330.0f, 530.0f))
                        : (TreeMesh == RuntimeFabTreeSmallMesh.Get()
                            ? Random.FRandRange(310.0f, 445.0f)
                            : Random.FRandRange(405.0f, 590.0f)),
                    Random.FRandRange(0.82f, 1.16f),
                    FRotator(0.0f, Random.FRandRange(0.0f, 360.0f), 0.0f));
            }
        }
    }

    const auto AddBush = [this, &Random, bHasCC0Bushes](const FVector& Location, float MinHeight, float MaxHeight, bool bFlowerBias)
    {
        UStaticMesh* BushMesh = nullptr;
        bool bSelectedCC0 = false;
        if (bHasCC0Bushes)
        {
            const int32 MeshIndex = Random.RandRange(0, RuntimeCC0BushMeshes.Num() - 1);
            BushMesh = RuntimeCC0BushMeshes[MeshIndex].Get();
            bSelectedCC0 = BushMesh != nullptr;
        }
        else
        {
            const float Choice = Random.FRand();
            if (RuntimeFabBushFlowerMesh && (Choice < (bFlowerBias ? 0.36f : 0.14f)))
            {
                BushMesh = RuntimeFabBushFlowerMesh.Get();
            }
            else if (RuntimeFabBushLargeMesh && Choice < 0.58f)
            {
                BushMesh = RuntimeFabBushLargeMesh.Get();
            }
            else if (RuntimeFabBushMediumMesh)
            {
                BushMesh = RuntimeFabBushMediumMesh.Get();
            }
            else
            {
                BushMesh = RuntimeFabBushLargeMesh
                    ? RuntimeFabBushLargeMesh.Get()
                    : RuntimeFabBushFlowerMesh.Get();
            }
        }
        if (!BushMesh)
        {
            return;
        }

        const FName BatchName = bSelectedCC0
            ? FName(*FString::Printf(TEXT("CC0_Bush_%s"), *BushMesh->GetName()))
            : FName(BushMesh == RuntimeFabBushFlowerMesh.Get() ? TEXT("FabFlowerBushInstances")
                : (BushMesh == RuntimeFabBushLargeMesh.Get() ? TEXT("FabLargeBushInstances") : TEXT("FabMediumBushInstances")));
        AddFoliageInstance(
            BushMesh,
            BatchName,
            Location,
            Random.FRandRange(MinHeight, MaxHeight),
            Random.FRandRange(0.78f, 1.35f),
            FRotator(0.0f, Random.FRandRange(0.0f, 360.0f), 0.0f));
    };

    if (bHasBushes)
    {
        const FVector CitySites[] = {
            FVector(-680.0f, -635.0f, 0.0f), FVector(-970.0f, 75.0f, 0.0f),
            FVector(760.0f, -690.0f, 0.0f), FVector(860.0f, 20.0f, 0.0f),
            FVector(-440.0f, -1110.0f, 0.0f), FVector(-1880.0f, -640.0f, 0.0f),
            FVector(-170.0f, 142.0f, 0.0f), FVector(-320.0f, -320.0f, 0.0f),
            FVector(310.0f, -330.0f, 0.0f), FVector(-360.0f, 360.0f, 0.0f),
            FVector(-2210.0f, -330.0f, 0.0f), FVector(-2050.0f, 720.0f, 0.0f)
        };
        const float CitySiteRadii[] = { 320.0f, 285.0f, 315.0f, 285.0f, 260.0f, 320.0f,
            210.0f, 190.0f, 190.0f, 190.0f, 210.0f, 260.0f };

        for (float GridX = -1260.0f; GridX <= 1260.0f; GridX += 220.0f)
        {
            for (float GridY = -1180.0f; GridY <= 1020.0f; GridY += 220.0f)
            {
                FVector Location(
                    GridX + Random.FRandRange(-72.0f, 72.0f),
                    GridY + Random.FRandRange(-72.0f, 72.0f),
                    0.0f);
                if (Random.FRand() > 0.40f
                    || IsNearPath(Location, RiverPath, 210.0f)
                    || IsNearPath(Location, MainRoad, 190.0f)
                    || IsNearPath(Location, FarmRoad, 170.0f)
                    || IsNearPath(Location, OrchardPath, 160.0f))
                {
                    continue;
                }

                bool bNearStructure = false;
                for (int32 SiteIndex = 0; SiteIndex < UE_ARRAY_COUNT(CitySites); ++SiteIndex)
                {
                    if (FVector2D::Distance(
                            FVector2D(Location.X, Location.Y),
                            FVector2D(CitySites[SiteIndex].X, CitySites[SiteIndex].Y)) < CitySiteRadii[SiteIndex])
                    {
                        bNearStructure = true;
                        break;
                    }
                }
                if (bNearStructure)
                {
                    continue;
                }
                AddBush(Location, 78.0f, 155.0f, true);
            }
        }

        for (float GridX = -3860.0f; GridX <= 3860.0f; GridX += 285.0f)
        {
            for (float GridY = -2860.0f; GridY <= 2860.0f; GridY += 285.0f)
            {
                const bool bVillageCore = FMath::Abs(GridX) < 1420.0f && FMath::Abs(GridY) < 1090.0f;
                if (bVillageCore || Random.FRand() > 0.48f)
                {
                    continue;
                }
                FVector Location(
                    GridX + Random.FRandRange(-90.0f, 90.0f),
                    GridY + Random.FRandRange(-90.0f, 90.0f),
                    0.0f);
                const bool bInFarmPlot =
                    (FMath::Abs(Location.X + 1980.0f) < 650.0f && FMath::Abs(Location.Y) < 470.0f)
                    || (FMath::Abs(Location.X + 1690.0f) < 650.0f && FMath::Abs(Location.Y + 1260.0f) < 430.0f)
                    || (FMath::Abs(Location.X + 760.0f) < 510.0f && FMath::Abs(Location.Y + 1430.0f) < 390.0f);
                const bool bNearLandmark =
                    FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(1050.0f, 1150.0f)) < 940.0f
                    || FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(1980.0f, 780.0f)) < 390.0f
                    || FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(-2050.0f, 720.0f)) < 340.0f
                    || (RuntimeFabMansionMesh
                        && FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(3050.0f, 1380.0f)) < 980.0f);
                if (bInFarmPlot || bNearLandmark
                    || IsNearPath(Location, RiverPath, 255.0f)
                    || IsNearPath(Location, MainRoad, 260.0f)
                    || IsNearPath(Location, FarmRoad, 200.0f)
                    || IsNearPath(Location, OrchardPath, 200.0f))
                {
                    continue;
                }
                AddBush(Location, 95.0f, 245.0f, false);
            }
        }
    }
}

void AAetherDevelopmentWorldActor::BuildLandmarks()
{
    const FVector Windmill(-2050.0f, 720.0f, 0.0f);
    AddPrimitive(RuntimeCylinderMesh, TEXT("WindmillTower"),
        Windmill + FVector(0.0f, 0.0f, 185.0f), FVector(1.08f, 1.08f, 3.55f),
        FLinearColor(0.83f, 0.79f, 0.66f, 1.0f), true);
    AddPrimitive(RuntimeConeMesh, TEXT("WindmillRoof"),
        Windmill + FVector(0.0f, 0.0f, 380.0f), FVector(1.15f, 1.15f, 0.94f),
        FLinearColor(0.60f, 0.22f, 0.13f, 1.0f));
    AddPrimitive(RuntimeSphereMesh, TEXT("WindmillHub"),
        Windmill + FVector(0.0f, -56.0f, 365.0f), FVector(0.35f, 0.35f, 0.35f),
        WarmStoneColor);
    AddPrimitive(RuntimeCubeMesh, TEXT("WindmillBladeHorizontal"),
        Windmill + FVector(0.0f, -60.0f, 365.0f), FVector(2.75f, 0.14f, 0.12f),
        FLinearColor(0.56f, 0.37f, 0.20f, 1.0f), false, FRotator(0.0f, 0.0f, 0.0f));
    AddPrimitive(RuntimeCubeMesh, TEXT("WindmillBladeDiagonalA"),
        Windmill + FVector(0.0f, -62.0f, 365.0f), FVector(2.65f, 0.14f, 0.12f),
        FLinearColor(0.60f, 0.41f, 0.23f, 1.0f), false, FRotator(0.0f, 42.0f, 0.0f));
    AddPrimitive(RuntimeCubeMesh, TEXT("WindmillBladeSailA"),
        Windmill + FVector(87.0f, -67.0f, 420.0f), FVector(0.78f, 0.16f, 0.10f),
        FLinearColor(0.86f, 0.80f, 0.65f, 1.0f), false, FRotator(0.0f, 42.0f, 0.0f));

    const FVector Ruins(1980.0f, 780.0f, 0.0f);
    AddPrimitive(RuntimeCubeMesh, TEXT("RuinFoundation"),
        Ruins + FVector(0.0f, 0.0f, 24.0f), FVector(5.2f, 4.0f, 0.42f),
        FLinearColor(0.47f, 0.45f, 0.38f, 1.0f));
    AddPrimitive(RuntimeCylinderMesh, TEXT("RuinPillarLeft"),
        Ruins + FVector(-170.0f, -90.0f, 145.0f), FVector(0.70f, 0.70f, 2.6f),
        FLinearColor(0.65f, 0.62f, 0.53f, 1.0f), true);
    AddPrimitive(RuntimeCylinderMesh, TEXT("RuinPillarRight"),
        Ruins + FVector(170.0f, -90.0f, 100.0f), FVector(0.70f, 0.70f, 1.8f),
        FLinearColor(0.59f, 0.57f, 0.49f, 1.0f), true);
    AddPrimitive(RuntimeCubeMesh, TEXT("RuinBrokenLintel"),
        Ruins + FVector(0.0f, -90.0f, 268.0f), FVector(3.8f, 0.55f, 0.42f),
        FLinearColor(0.67f, 0.64f, 0.54f, 1.0f), false, FRotator(0.0f, 0.0f, -8.0f));
    AddPrimitive(RuntimeCubeMesh, TEXT("RuinWallRemnant"),
        Ruins + FVector(-255.0f, 75.0f, 105.0f), FVector(0.62f, 2.6f, 1.8f),
        FLinearColor(0.55f, 0.53f, 0.46f, 1.0f), true, FRotator(0.0f, 0.0f, -6.0f));

    AddPrimitive(RuntimeCylinderMesh, TEXT("FrontierWatchtower"),
        FVector(-2040.0f, 1260.0f, 205.0f), FVector(0.90f, 0.90f, 4.0f),
        FLinearColor(0.61f, 0.51f, 0.36f, 1.0f), true);
    AddPrimitive(RuntimeConeMesh, TEXT("FrontierWatchtowerRoof"),
        FVector(-2040.0f, 1260.0f, 425.0f), FVector(1.0f, 1.0f, 0.82f),
        FLinearColor(0.45f, 0.18f, 0.13f, 1.0f));
}

void AAetherDevelopmentWorldActor::BuildFabMansionLandmark()
{
    if (!RuntimeFabMansionMesh || !Root)
    {
        return;
    }

    const FBox Bounds = RuntimeFabMansionMesh->GetBoundingBox();
    const FVector Size = Bounds.GetSize();
    const float HorizontalSize = FMath::Max(Size.X, Size.Y);
    if (!Bounds.IsValid || HorizontalSize <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogTemp, Warning, TEXT("Fab mansion mesh has invalid bounds; not placing it."));
        return;
    }

    // GLB geometry is normalized around a unit box. Fit it by world width rather
    // than applying a guessed fixed multiplier (the imported mesh may be 1 or 100 cm wide).
    const float UniformScale = 1850.0f / HorizontalSize;
    const FRotator Rotation(0.0f, -12.0f, 0.0f);
    const FVector CenterLocation(3050.0f, 1380.0f, 0.0f);
    const FVector MeshCenter = Bounds.GetCenter();
    const FVector LocalOffset(
        -MeshCenter.X * UniformScale,
        -MeshCenter.Y * UniformScale,
        -Bounds.Min.Z * UniformScale);
    const FVector Location = CenterLocation + Rotation.RotateVector(LocalOffset);

    RuntimeFabMansionComponent = NewObject<UStaticMeshComponent>(
        this, MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), TEXT("FabHauntedMansion")));
    if (!RuntimeFabMansionComponent)
    {
        return;
    }
    RuntimeFabMansionComponent->SetupAttachment(Root);
    RuntimeFabMansionComponent->SetStaticMesh(RuntimeFabMansionMesh);
    RuntimeFabMansionComponent->SetRelativeLocation(Location);
    RuntimeFabMansionComponent->SetRelativeRotation(Rotation);
    RuntimeFabMansionComponent->SetRelativeScale3D(FVector(UniformScale));
    RuntimeFabMansionComponent->SetMobility(EComponentMobility::Movable);
    RuntimeFabMansionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RuntimeFabMansionComponent->SetCollisionProfileName(TEXT("NoCollision"));
    RuntimeFabMansionComponent->SetCanEverAffectNavigation(false);
    RuntimeFabMansionComponent->SetCastShadow(true);
    AddInstanceComponent(RuntimeFabMansionComponent);
    RuntimeFabMansionComponent->RegisterComponent();
    RuntimeVisualComponents.Add(RuntimeFabMansionComponent);
    UE_LOG(LogTemp, Log, TEXT("Placed Fab mansion at forest edge; uniform scale %.2f (target width 18.5 m)."), UniformScale);
}

void AAetherDevelopmentWorldActor::BuildFabHorseAtStable()
{
    if (!RuntimeFabHorseMesh || !Root)
    {
        return;
    }

    RuntimeFabHorseComponent = NewObject<USkeletalMeshComponent>(
        this, MakeUniqueObjectName(this, USkeletalMeshComponent::StaticClass(), TEXT("FabUnicornAtStable")));
    if (!RuntimeFabHorseComponent)
    {
        return;
    }
    RuntimeFabHorseComponent->SetupAttachment(Root);
    RuntimeFabHorseComponent->SetSkeletalMesh(RuntimeFabHorseMesh);
    RuntimeFabHorseComponent->SetRelativeLocation(FVector(-2170.0f, -705.0f, 5.0f));
    RuntimeFabHorseComponent->SetRelativeRotation(FRotator(0.0f, 18.0f, 0.0f));
    RuntimeFabHorseComponent->SetRelativeScale3D(FVector(0.90f));
    RuntimeFabHorseComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RuntimeFabHorseComponent->SetCanEverAffectNavigation(false);
    RuntimeFabHorseComponent->SetCastShadow(true);
    RuntimeFabHorseComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    AddInstanceComponent(RuntimeFabHorseComponent);
    RuntimeFabHorseComponent->RegisterComponent();
    if (RuntimeFabHorseIdleAnimation)
    {
        RuntimeFabHorseComponent->PlayAnimation(RuntimeFabHorseIdleAnimation, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Fab horse mesh is present but the Idle animation asset is missing."));
    }
}

void AAetherDevelopmentWorldActor::BuildAmbientVillageNPCs()
{
    if (RuntimeAmbientNPCComponents.Num() > 0)
    {
        return;
    }

    USkeletalMesh* WalkerMesh = LoadObject<USkeletalMesh>(
        nullptr, TEXT("/Game/Aether/Characters/Mage/SK_Mago_AgeOfAether.SK_Mago_AgeOfAether"));
    RuntimeAmbientNPCIdleAnimation = LoadObject<UAnimSequence>(
        nullptr, TEXT("/Game/Aether/Characters/Mage/Animations/A_Idle_Anim.A_Idle_Anim"));
    if (!RuntimeAmbientNPCIdleAnimation)
    {
        RuntimeAmbientNPCIdleAnimation = LoadObject<UAnimSequence>(
            nullptr, TEXT("/Game/Aether/Characters/Mage/Animations/A_Idle.A_Idle"));
    }
    RuntimeAmbientNPCWalkAnimation = LoadObject<UAnimSequence>(
        nullptr, TEXT("/Game/Aether/Characters/Mage/Animations/A_Walk_Anim.A_Walk_Anim"));
    if (!RuntimeAmbientNPCWalkAnimation)
    {
        RuntimeAmbientNPCWalkAnimation = LoadObject<UAnimSequence>(
            nullptr, TEXT("/Game/Aether/Characters/Mage/Animations/A_Walk.A_Walk"));
    }
    if (!WalkerMesh || !RuntimeAmbientNPCWalkAnimation)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ambient Mage walkers were not spawned: import the Mage skeletal mesh and Walk sequence first."));
        return;
    }
    if (!RuntimeAmbientNPCIdleAnimation)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ambient Mage Idle sequence is missing; walkers will pause with the Walk sequence held."));
    }

    // Give each walker many reachable destinations along the town, farm, orchard,
    // and castle paths instead of sending the whole group around one closed loop.
    RuntimeAmbientWalkWaypoints = {
        FVector(0.0f, -1740.0f, 0.0f), FVector(-40.0f, -1190.0f, 0.0f),
        FVector(-130.0f, -700.0f, 0.0f), FVector(-80.0f, -280.0f, 0.0f),
        FVector(0.0f, 0.0f, 0.0f), FVector(-60.0f, 280.0f, 0.0f),
        FVector(120.0f, 490.0f, 0.0f), FVector(350.0f, 720.0f, 0.0f),
        FVector(420.0f, 540.0f, 0.0f), FVector(700.0f, 520.0f, 0.0f),
        FVector(970.0f, 580.0f, 0.0f), FVector(1050.0f, 695.0f, 0.0f),
        FVector(1050.0f, 1080.0f, 0.0f),
        FVector(-120.0f, -270.0f, 0.0f), FVector(-430.0f, -365.0f, 0.0f),
        FVector(-780.0f, -425.0f, 0.0f), FVector(-1160.0f, -500.0f, 0.0f),
        FVector(-1560.0f, -565.0f, 0.0f), FVector(-1930.0f, -780.0f, 0.0f),
        FVector(-450.0f, 100.0f, 0.0f), FVector(-760.0f, 380.0f, 0.0f),
        FVector(-1160.0f, 640.0f, 0.0f), FVector(-1530.0f, 810.0f, 0.0f)
    };
    RuntimeAmbientRandomStream.Initialize(FMath::Rand());
    RuntimeAmbientNPCComponents.Reset();
    RuntimeAmbientNPCWalkSpeeds.Reset();
    RuntimeAmbientNPCGroundOffsets.Reset();
    RuntimeAmbientNPCPauseTimers.Reset();
    RuntimeAmbientNPCTargets.Reset();

    constexpr int32 WalkerCount = 10;
    for (int32 WalkerIndex = 0; WalkerIndex < WalkerCount; ++WalkerIndex)
    {
        USkeletalMeshComponent* Walker = NewObject<USkeletalMeshComponent>(
            this,
            MakeUniqueObjectName(
                this, USkeletalMeshComponent::StaticClass(),
                FName(*FString::Printf(TEXT("AmbientTownWalker_%02d"), WalkerIndex))));
        if (!Walker)
        {
            continue;
        }

        FVector StartLocation = RuntimeAmbientWalkWaypoints[
            RuntimeAmbientRandomStream.RandRange(0, RuntimeAmbientWalkWaypoints.Num() - 1)];
        StartLocation.X += RuntimeAmbientRandomStream.FRandRange(-65.0f, 65.0f);
        StartLocation.Y += RuntimeAmbientRandomStream.FRandRange(-65.0f, 65.0f);
        StartLocation.Z = 4.0f;

        Walker->SetupAttachment(Root);
        Walker->SetSkeletalMesh(WalkerMesh);
        Walker->SetRelativeLocation(StartLocation);
        Walker->SetRelativeScale3D(FVector(0.84f + static_cast<float>(WalkerIndex % 3) * 0.06f));
        Walker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Walker->SetCanEverAffectNavigation(false);
        Walker->SetCastShadow(true);
        Walker->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        AddInstanceComponent(Walker);
        Walker->RegisterComponent();

        const float WalkerWalkSpeed = RuntimeAmbientRandomStream.FRandRange(82.0f, 124.0f);
        RuntimeAmbientNPCComponents.Add(Walker);
        RuntimeAmbientNPCGroundOffsets.Add(4.0f);
        RuntimeAmbientNPCWalkSpeeds.Add(WalkerWalkSpeed);
        RuntimeAmbientNPCPauseTimers.Add(RuntimeAmbientRandomStream.FRandRange(0.0f, 1.8f));
        RuntimeAmbientNPCTargets.Add(ChooseAmbientNPCDestination(StartLocation));
        PlayLoopingSkeletalAnimation(
            Walker,
            RuntimeAmbientNPCWalkAnimation,
            GetAmbientNPCWalkPlayRate(WalkerWalkSpeed));
    }

    if (RuntimeAmbientNPCComponents.Num() > 0)
    {
        SetActorTickEnabled(true);
    }
    UE_LOG(LogTemp, Log,
        TEXT("Spawned %d ambient Mage walkers with individual random destinations and Idle/Walk animation."),
        RuntimeAmbientNPCComponents.Num());
}

FVector AAetherDevelopmentWorldActor::ChooseAmbientNPCDestination(const FVector& FromLocal)
{
    if (RuntimeAmbientWalkWaypoints.Num() == 0)
    {
        return FromLocal;
    }

    constexpr int32 RandomAttempts = 64;
    for (int32 Attempt = 0; Attempt < RandomAttempts; ++Attempt)
    {
        FVector Candidate = RuntimeAmbientWalkWaypoints[
            RuntimeAmbientRandomStream.RandRange(0, RuntimeAmbientWalkWaypoints.Num() - 1)];
        Candidate.X += RuntimeAmbientRandomStream.FRandRange(-95.0f, 95.0f);
        Candidate.Y += RuntimeAmbientRandomStream.FRandRange(-95.0f, 95.0f);
        const float DistanceSquared = FVector::DistSquared2D(FromLocal, Candidate);
        if (DistanceSquared < FMath::Square(280.0f) || DistanceSquared > FMath::Square(1450.0f))
        {
            continue;
        }
        if (IsAmbientNPCPathClear(FromLocal, Candidate))
        {
            return Candidate;
        }
    }

    // If random candidates were blocked, step toward the nearest clear path node
    // rather than leaving a walker frozen in place.
    TArray<int32> NearestWaypointIndices;
    NearestWaypointIndices.Reserve(RuntimeAmbientWalkWaypoints.Num());
    for (int32 Index = 0; Index < RuntimeAmbientWalkWaypoints.Num(); ++Index)
    {
        NearestWaypointIndices.Add(Index);
    }
    NearestWaypointIndices.Sort([this, &FromLocal](int32 A, int32 B)
    {
        return FVector::DistSquared2D(FromLocal, RuntimeAmbientWalkWaypoints[A])
            < FVector::DistSquared2D(FromLocal, RuntimeAmbientWalkWaypoints[B]);
    });
    for (const int32 Index : NearestWaypointIndices)
    {
        const FVector& Candidate = RuntimeAmbientWalkWaypoints[Index];
        if (FVector::DistSquared2D(FromLocal, Candidate) > FMath::Square(160.0f)
            && IsAmbientNPCPathClear(FromLocal, Candidate))
        {
            return Candidate;
        }
    }
    return FromLocal;
}

bool AAetherDevelopmentWorldActor::IsAmbientNPCPathClear(
    const FVector& StartLocal,
    const FVector& EndLocal) const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    const FTransform ActorTransform = GetActorTransform();
    const FVector Start = ActorTransform.TransformPosition(StartLocal + FVector(0.0f, 0.0f, 70.0f));
    const FVector End = ActorTransform.TransformPosition(EndLocal + FVector(0.0f, 0.0f, 70.0f));
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(AetherAmbientNPCPath), false);
    FHitResult Hit;
    return !World->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, QueryParams);
}

void AAetherDevelopmentWorldActor::UpdateAmbientVillageNPCs(float DeltaSeconds)
{
    const int32 WalkerCount = RuntimeAmbientNPCComponents.Num();
    for (int32 WalkerIndex = 0; WalkerIndex < WalkerCount; ++WalkerIndex)
    {
        USkeletalMeshComponent* Walker = RuntimeAmbientNPCComponents[WalkerIndex].Get();
        if (!Walker || !RuntimeAmbientNPCTargets.IsValidIndex(WalkerIndex)
            || !RuntimeAmbientNPCWalkSpeeds.IsValidIndex(WalkerIndex)
            || !RuntimeAmbientNPCPauseTimers.IsValidIndex(WalkerIndex))
        {
            continue;
        }

        float& PauseTimer = RuntimeAmbientNPCPauseTimers[WalkerIndex];
        if (PauseTimer > 0.0f)
        {
            PauseTimer = FMath::Max(0.0f, PauseTimer - DeltaSeconds);
            if (PauseTimer <= 0.0f)
            {
                RuntimeAmbientNPCTargets[WalkerIndex] = ChooseAmbientNPCDestination(Walker->GetRelativeLocation());
                if (RuntimeAmbientNPCWalkAnimation)
                {
                    PlayLoopingSkeletalAnimation(
                        Walker,
                        RuntimeAmbientNPCWalkAnimation,
                        GetAmbientNPCWalkPlayRate(RuntimeAmbientNPCWalkSpeeds[WalkerIndex]));
                }
            }
            continue;
        }

        const FVector CurrentLocation = Walker->GetRelativeLocation();
        FVector ToTarget = RuntimeAmbientNPCTargets[WalkerIndex] - CurrentLocation;
        ToTarget.Z = 0.0f;
        if (ToTarget.SizeSquared2D() <= FMath::Square(58.0f))
        {
            PauseTimer = RuntimeAmbientRandomStream.FRandRange(0.8f, 3.6f);
            if (RuntimeAmbientNPCIdleAnimation)
            {
                PlayLoopingSkeletalAnimation(Walker, RuntimeAmbientNPCIdleAnimation);
            }
            continue;
        }

        const FVector Direction = ToTarget.GetSafeNormal2D();
        const float StepDistance = FMath::Min(
            RuntimeAmbientNPCWalkSpeeds[WalkerIndex] * DeltaSeconds,
            ToTarget.Size2D());
        FVector NewLocation = CurrentLocation + Direction * StepDistance;
        NewLocation.Z = RuntimeAmbientNPCGroundOffsets.IsValidIndex(WalkerIndex)
            ? RuntimeAmbientNPCGroundOffsets[WalkerIndex]
            : 4.0f;
        Walker->SetRelativeLocation(NewLocation);
        // Keep the Mage's imported local forward axis aligned with the travel vector.
        Walker->SetRelativeRotation(FRotator(0.0f, Direction.Rotation().Yaw - 90.0f, 0.0f));
    }
}

void AAetherDevelopmentWorldActor::BuildFirstRegionDiorama()
{
    if (bDioramaBuilt || !GetWorld())
    {
        return;
    }

    if (!RuntimeCubeMesh || !RuntimeCylinderMesh || !RuntimeSphereMesh || !RuntimeConeMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Age of Aether first-region diorama could not load Unreal BasicShapes."));
        return;
    }

    LoadEnvironmentAssets();
    bDioramaBuilt = true;

    BuildTerrain();
    BuildGroundCover();
    BuildRiverAndRoads();
    BuildFarms();
    BuildVillage();
    BuildBridge();
    BuildCastle();
    BuildForest();
    BuildImportedVegetation();
    BuildLandmarks();
    BuildFabMansionLandmark();
    if (bPlaceIdleFabHorseAtStable)
    {
        BuildFabHorseAtStable();
    }
    if (bEnableMageWalkerPlaceholder)
    {
        BuildAmbientVillageNPCs();
    }

    UE_LOG(LogTemp, Log, TEXT("Age of Aether first-region diorama built: river, bridge, farms, village, castle, authored forest vegetation and landmarks."));
}
