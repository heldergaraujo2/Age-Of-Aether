#include "World/Aether2DDepthLightingComponent.h"

#include "Camera/PlayerCameraManager.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"

UAether2DDepthLightingComponent::UAether2DDepthLightingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
}

void UAether2DDepthLightingComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bApplyOnBeginPlay && Profile)
    {
        ApplyProfile();
    }

    if (AActor* Owner = GetOwner())
    {
        InitialOwnerLocation = Owner->GetActorLocation();

        if (UWorld* World = GetWorld())
        {
            if (APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(World, 0))
            {
                InitialCameraLocation = Camera->GetCameraLocation();
            }
        }
    }
}

void UAether2DDepthLightingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Profile && Profile->bEnableParallax && GetNetMode() != NM_DedicatedServer)
    {
        ApplyParallax();
    }
}

bool UAether2DDepthLightingComponent::ApplyProfile()
{
    if (!Profile || GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    return ApplyLoadedProfile(Profile);
}

bool UAether2DDepthLightingComponent::ApplyProfileAsset(UAether2DDepthLightingProfile* InProfile)
{
    if (!InProfile)
    {
        return false;
    }

    Profile = InProfile;
    return ApplyProfile();
}

bool UAether2DDepthLightingComponent::ApplyLoadedProfile(UAether2DDepthLightingProfile* InProfile)
{
    FString ValidationError;
    if (!InProfile->ValidateProfile(ValidationError))
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether 2D depth profile rejected: %s"), *ValidationError);
        return false;
    }

    if (UPrimitiveComponent* Presentation = ResolvePresentationComponent())
    {
        Presentation->TranslucencySortPriority = InProfile->RenderLayer;
        Presentation->SetCastShadow(InProfile->bCastShadow);
    }

    return true;
}

void UAether2DDepthLightingComponent::ApplyParallax()
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    if (!Owner || !World)
    {
        return;
    }

    APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(World, 0);
    if (!Camera)
    {
        return;
    }

    const FVector CameraDelta = Camera->GetCameraLocation() - InitialCameraLocation;
    const FVector ParallaxDelta = CameraDelta * (1.0f - Profile->ParallaxFactor);
    const FVector BaseLocation = InitialOwnerLocation + FVector(0.0f, 0.0f, Profile->DepthOffset);

    Owner->SetActorLocation(BaseLocation + ParallaxDelta);
}

TObjectPtr<UPrimitiveComponent> UAether2DDepthLightingComponent::ResolvePresentationComponent() const
{
    if (AActor* Owner = GetOwner())
    {
        TArray<UPrimitiveComponent*> Components;
        Owner->GetComponents<UPrimitiveComponent>(Components);

        for (UPrimitiveComponent* Component : Components)
        {
            if (Component && Component->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
            {
                return Component;
            }
        }
    }

    return nullptr;
}
