#include "Characters/AetherPlayableCharacterVisualComponent.h"

#include "Characters/AetherCharacter.h"
#include "Characters/AetherPlayableCharacterVisualProfile.h"
#include "Characters/AetherCharacterAnimationProfile.h"
#include "Characters/AetherBaseAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInterface.h"

UAetherPlayableCharacterVisualComponent::UAetherPlayableCharacterVisualComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAetherPlayableCharacterVisualComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bApplyOnBeginPlay)
    {
        ApplyProfile();
    }
}

bool UAetherPlayableCharacterVisualComponent::ApplyProfile()
{
    if (!Profile)
    {
        return false;
    }

    if (GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    return ApplyLoadedProfile(Profile);
}

bool UAetherPlayableCharacterVisualComponent::ApplyProfileAsset(UAetherPlayableCharacterVisualProfile* InProfile)
{
    if (!InProfile)
    {
        return false;
    }

    Profile = InProfile;
    return ApplyProfile();
}

USkeletalMeshComponent* UAetherPlayableCharacterVisualComponent::GetMeshComponent() const
{
    const AAetherCharacter* Character = Cast<AAetherCharacter>(GetOwner());
    return Character ? Character->GetMesh() : nullptr;
}

bool UAetherPlayableCharacterVisualComponent::ApplyLoadedProfile(UAetherPlayableCharacterVisualProfile* InProfile)
{
    FString ValidationError;
    if (!InProfile->ValidateProfile(ValidationError))
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether visual profile rejected: %s"), *ValidationError);
        return false;
    }

    USkeletalMeshComponent* MeshComponent = GetMeshComponent();
    if (!MeshComponent)
    {
        return false;
    }

    USkeletalMesh* Mesh = InProfile->SkeletalMesh.LoadSynchronous();
    if (!Mesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether visual profile '%s' could not load skeletal mesh."), *InProfile->VisualProfileID.ToString());
        return false;
    }

    MeshComponent->SetSkeletalMesh(Mesh);
    MeshComponent->SetRelativeTransform(InProfile->MeshRelativeTransform);

    TSoftClassPtr<UAnimInstance> AnimationClass = InProfile->AnimationClass;
    if (AnimationClass.IsNull() && InProfile->AnimationProfile)
    {
        AnimationClass = InProfile->AnimationProfile->AnimationClass;
    }

    if (!AnimationClass.IsNull())
    {
        if (UClass* AnimClass = AnimationClass.LoadSynchronous())
        {
            MeshComponent->SetAnimInstanceClass(AnimClass);
        }
    }

    if (InProfile->AnimationProfile)
    {
        if (UAetherBaseAnimInstance* AnimInstance = Cast<UAetherBaseAnimInstance>(MeshComponent->GetAnimInstance()))
        {
            AnimInstance->AnimationProfile = InProfile->AnimationProfile;
        }
    }

    MeshComponent->EmptyOverrideMaterials();
    for (int32 Index = 0; Index < InProfile->MaterialOverrides.Num(); ++Index)
    {
        if (UMaterialInterface* Material = InProfile->MaterialOverrides[Index].LoadSynchronous())
        {
            MeshComponent->SetMaterial(Index, Material);
        }
    }

    return true;
}
