#include "World/AetherDevelopmentWorldActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
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
}

AAetherDevelopmentWorldActor::AAetherDevelopmentWorldActor()
{
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

void AAetherDevelopmentWorldActor::ConfigureGround()
{
    if (!Ground)
    {
        return;
    }

    Ground->SetRelativeScale3D(GroundScale);
    Ground->SetRelativeLocation(FVector(0.0f, 0.0f, GroundZ));
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
    const FRotator& Rotation)
{
    if (!Mesh || !Root)
    {
        return;
    }

    const uint32 ColorKey = Color.ToFColor(true).DWColor();
    const uint32 ComponentKey = HashCombine(
        HashCombine(GetTypeHash(Mesh), ColorKey),
        bBlockMovement ? 1u : 0u);

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
        Component->SetCastShadow(true);

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

    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_CanopyShadow"), *Prefix)),
        Location + FVector(0.0f, 0.0f, Height * 0.70f),
        FVector(2.75f * Scale, 2.55f * Scale, 2.40f * Scale),
        LeafDark);
    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_CanopyLeft"), *Prefix)),
        Location + FVector(-78.0f * Scale, -7.0f * Scale, Height * 0.73f),
        FVector(1.95f * Scale, 1.78f * Scale, 1.88f * Scale),
        CanopyMain);
    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_CanopyRight"), *Prefix)),
        Location + FVector(82.0f * Scale, 16.0f * Scale, Height * 0.73f),
        FVector(1.92f * Scale, 1.82f * Scale, 1.86f * Scale),
        CanopyMain * FLinearColor(0.90f, 0.98f, 0.88f, 1.0f));
    AddInstancedPrimitive(
        RuntimeSphereMesh,
        FName(*FString::Printf(TEXT("%s_CanopyTop"), *Prefix)),
        Location + FVector(-8.0f * Scale, 18.0f * Scale, Height * 0.94f),
        FVector(1.88f * Scale, 1.75f * Scale, 1.78f * Scale),
        CanopyAccent);
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
        if (UMaterialInstanceDynamic* Material = CreateColorMaterial(GrassColor))
        {
            Ground->SetMaterial(0, Material);
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

    bDioramaBuilt = true;

    BuildTerrain();
    BuildRiverAndRoads();
    BuildFarms();
    BuildVillage();
    BuildBridge();
    BuildCastle();
    BuildForest();
    BuildLandmarks();

    UE_LOG(LogTemp, Log, TEXT("Age of Aether first-region diorama built: river, bridge, farms, village, castle, forest and landmarks."));
}
