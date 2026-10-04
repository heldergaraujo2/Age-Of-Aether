#include "World/Aether2DEnvironmentVisualComponent.h"

#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

UAether2DEnvironmentVisualComponent::UAether2DEnvironmentVisualComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAether2DEnvironmentVisualComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bApplyOnBeginPlay && Profile)
    {
        ApplyProfile();
    }
}

bool UAether2DEnvironmentVisualComponent::ApplyProfile()
{
    if (!Profile || GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    return ApplyLoadedProfile(Profile);
}

bool UAether2DEnvironmentVisualComponent::ApplyProfileAsset(UAether2DEnvironmentVisualProfile* InProfile)
{
    if (!InProfile)
    {
        return false;
    }

    Profile = InProfile;
    return ApplyProfile();
}

bool UAether2DEnvironmentVisualComponent::ApplyLoadedProfile(UAether2DEnvironmentVisualProfile* InProfile)
{
    FString ValidationError;
    if (!InProfile->ValidateProfile(ValidationError))
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether 2D environment visual profile rejected: %s"), *ValidationError);
        return false;
    }

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return false;
    }

    if (!SpriteComponent)
    {
        SpriteComponent = NewObject<UPaperSpriteComponent>(Owner, TEXT("Aether2DEnvironmentSprite"));
        SpriteComponent->RegisterComponent();
        SpriteComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

    UPaperSprite* Sprite = InProfile->Sprite.LoadSynchronous();
    if (!Sprite)
    {
        return false;
    }

    SpriteComponent->SetSprite(Sprite);
    SpriteComponent->SetRelativeLocation(InProfile->SpriteWorldOffset);
    SpriteComponent->SetRelativeScale3D(FVector(InProfile->VisualScale.X, InProfile->VisualScale.Y, 1.0f));
    SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SpriteComponent->SetCastShadow(InProfile->bCastShadow);
    SpriteComponent->TranslucencySortPriority = InProfile->RenderLayer;
    return true;
}
