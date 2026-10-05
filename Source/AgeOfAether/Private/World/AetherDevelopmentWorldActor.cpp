#include "World/AetherDevelopmentWorldActor.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "World/AetherRuntime2DArt.h"

AAetherDevelopmentWorldActor::AAetherDevelopmentWorldActor()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    Root->SetMobility(EComponentMobility::Static);
    SetRootComponent(Root);

    Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
    Ground->SetupAttachment(Root);
    Ground->SetCollisionProfileName(TEXT("BlockAll"));
    Ground->SetMobility(EComponentMobility::Static);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Ground->SetStaticMesh(CubeMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
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
    Ground->SetRelativeLocation(FVector(0.0, 0.0, GroundZ));
}

UMaterialInstanceDynamic* AAetherDevelopmentWorldActor::CreateColorMaterial(const FLinearColor& Color) const
{
    if (!RuntimeBaseMaterial)
    {
        return nullptr;
    }

    UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(
        RuntimeBaseMaterial,
        const_cast<AAetherDevelopmentWorldActor*>(this));

    if (Material)
    {
        Material->SetVectorParameterValue(TEXT("Color"), Color);
    }

    return Material;
}

UStaticMeshComponent* AAetherDevelopmentWorldActor::AddPrimitive(
    UStaticMesh* Mesh,
    const FName& Name,
    const FVector& Location,
    const FVector& Scale,
    const FLinearColor& Color,
    bool bBlockMovement)
{
    if (!Mesh)
    {
        return nullptr;
    }

    UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this, Name);
    if (!Component)
    {
        return nullptr;
    }

    Component->SetupAttachment(Root);
    Component->SetStaticMesh(Mesh);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeScale3D(Scale);
    Component->SetMobility(EComponentMobility::Static);
    Component->SetCollisionEnabled(
        bBlockMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

    if (UMaterialInstanceDynamic* Material = CreateColorMaterial(Color))
    {
        Component->SetMaterial(0, Material);
    }

    Component->RegisterComponent();
    RuntimeVisualComponents.Add(Component);
    return Component;
}

UPaperSpriteComponent* AAetherDevelopmentWorldActor::AddSpriteArt(
    UPaperSprite* Sprite,
    const FName& Name,
    const FVector& Location,
    float Scale,
    float Yaw)
{
    if (!Sprite)
    {
        return nullptr;
    }

    UPaperSpriteComponent* Component = NewObject<UPaperSpriteComponent>(this, Name);
    if (!Component)
    {
        return nullptr;
    }

    Component->SetupAttachment(Root);
    Component->SetSprite(Sprite);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeRotation(FRotator(0.0f, Yaw, 0.0f));
    Component->SetRelativeScale3D(FVector(Scale));
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Component->SetCastShadow(false);
    Component->SetMobility(EComponentMobility::Movable);

    AddInstanceComponent(Component);
    RuntimeSpriteComponents.Add(Component);
    RuntimeSprites.Add(Sprite);
    if (UTexture2D* SourceTexture = Sprite->GetSourceTexture())
    {
        RuntimeSpriteTextures.Add(SourceTexture);
    }

    Component->RegisterComponent();
    Component->SetSpriteColor(FLinearColor::White);
    Component->SetVisibility(true, true);
    Component->MarkRenderStateDirty();
    return Component;
}

void AAetherDevelopmentWorldActor::BuildFirstRegionDiorama()
{
    if (bDioramaBuilt || !GetWorld())
    {
        return;
    }

    bDioramaBuilt = true;

    static UStaticMesh* CubeMesh = nullptr;
    static UStaticMesh* CylinderMesh = nullptr;
    static UStaticMesh* SphereMesh = nullptr;
    static UStaticMesh* ConeMesh = nullptr;

    if (!CubeMesh)
    {
        CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
        CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
        SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
        ConeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
    }

    const FLinearColor Grass(0.12f, 0.28f, 0.10f, 1.0f);
    const FLinearColor GrassAlt(0.18f, 0.36f, 0.12f, 1.0f);
    const FLinearColor Stone(0.30f, 0.32f, 0.34f, 1.0f);
    const FLinearColor Road(0.26f, 0.20f, 0.14f, 1.0f);
    const FLinearColor Wood(0.25f, 0.10f, 0.045f, 1.0f);
    const FLinearColor Roof(0.38f, 0.07f, 0.045f, 1.0f);
    const FLinearColor Leaf(0.05f, 0.24f, 0.08f, 1.0f);
    const FLinearColor Water(0.04f, 0.20f, 0.32f, 1.0f);
    const FLinearColor Gold(0.72f, 0.48f, 0.08f, 1.0f);

    if (Ground)
    {
        if (UMaterialInstanceDynamic* Material = CreateColorMaterial(Grass))
        {
            Ground->SetMaterial(0, Material);
        }
    }

    AddPrimitive(CubeMesh, TEXT("Plaza"), FVector(0, 0, 8), FVector(7.5, 6.0, 0.12), Stone, true);
    AddPrimitive(CubeMesh, TEXT("Road"), FVector(0, 850, 10), FVector(3.0, 10.0, 0.10), Road, true);
    AddPrimitive(CubeMesh, TEXT("RoadBranch"), FVector(650, 300, 10), FVector(7.0, 1.6, 0.10), Road, true);
    AddPrimitive(CubeMesh, TEXT("Water"), FVector(-1050, -350, 4), FVector(5.0, 3.0, 0.06), Water, false);

    const FVector HouseLocations[] = {
        FVector(-650, -100, 120), FVector(650, -150, 120), FVector(-520, 620, 120)
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(HouseLocations); ++Index)
    {
        const FVector Location = HouseLocations[Index];
        AddSpriteArt(
            FAetherRuntime2DArt::CreateHouseSprite(this, Index),
            FName(*FString::Printf(TEXT("HouseArt_%d"), Index)),
            Location + FVector(0, 0, 178),
            1.0f,
            45.0f);
    }

    const FVector TreeLocations[] = {
        FVector(-1050, 650, 0), FVector(-850, 1050, 0), FVector(1050, 650, 0),
        FVector(900, 1050, 0), FVector(1250, -50, 0), FVector(-1200, 50, 0),
        FVector(1200, 1050, 0), FVector(-1050, -900, 0)
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(TreeLocations); ++Index)
    {
        const FVector Location = TreeLocations[Index];
        AddSpriteArt(
            FAetherRuntime2DArt::CreateTreeSprite(this, Index % 2),
            FName(*FString::Printf(TEXT("TreeArt_%d"), Index)),
            Location + FVector(0, 0, 128),
            1.55f,
            45.0f);
    }

    const FVector RockLocations[] = {
        FVector(-250, 950, 40), FVector(300, 1050, 55), FVector(850, 850, 45),
        FVector(-850, 350, 35), FVector(950, -650, 50), FVector(-300, -900, 45)
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(RockLocations); ++Index)
    {
        AddSpriteArt(
            FAetherRuntime2DArt::CreateRockSprite(this, Index % 2),
            FName(*FString::Printf(TEXT("RockArt_%d"), Index)),
            RockLocations[Index] + FVector(0, 0, 48),
            1.15f,
            45.0f);
    }

    AddPrimitive(CubeMesh, TEXT("GateLeft"), FVector(-300, 700, 220), FVector(0.5, 0.8, 4.0), Stone, true);
    AddPrimitive(CubeMesh, TEXT("GateRight"), FVector(300, 700, 220), FVector(0.5, 0.8, 4.0), Stone, true);
    AddPrimitive(CubeMesh, TEXT("GateTop"), FVector(0, 700, 560), FVector(3.5, 0.8, 0.5), Stone, true);
    AddPrimitive(CylinderMesh, TEXT("Landmark"), FVector(0, -850, 180), FVector(1.4, 1.4, 3.6), Gold, true);
}
