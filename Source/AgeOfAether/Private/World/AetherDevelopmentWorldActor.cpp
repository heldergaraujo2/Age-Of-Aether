#include "World/AetherDevelopmentWorldActor.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AAetherDevelopmentWorldActor::AAetherDevelopmentWorldActor()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
    Ground->SetupAttachment(Root);
    Ground->SetCollisionProfileName(TEXT("BlockAll"));
    Ground->SetMobility(EComponentMobility::Static);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Ground->SetStaticMesh(CubeMesh.Object);
    }

    ConfigureGround();
}

void AAetherDevelopmentWorldActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    ConfigureGround();
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
