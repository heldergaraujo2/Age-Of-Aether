#include "Characters/AetherSkillVisualComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"

void UAetherSkillVisualComponent::PlaySkillPresentation(const FAetherSkillVisualProfile& Profile, bool bImpact)
{
    if (GetOwner() && GetOwner()->GetNetMode() == NM_DedicatedServer) return;
    if (!Profile.IsValid()) return;

    TSoftObjectPtr<UObject> Asset = bImpact ? Profile.ImpactVFX : Profile.CastVFX;
    if (Asset.IsValid())
    {
        if (UObject* Loaded = Asset.LoadSynchronous())
            ActiveVisualObjects.Add(Loaded);
    }

    if (!bImpact && Profile.CastMontage.IsValid())
    {
        if (UAnimMontage* Montage = Cast<UAnimMontage>(Profile.CastMontage.LoadSynchronous()))
        {
            if (USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
            {
                if (UAnimInstance* Anim = Mesh->GetAnimInstance())
                    Anim->Montage_Play(Montage);
            }
        }
    }

    if (Profile.CastSFX.IsValid())
    {
        if (UObject* Loaded = Profile.CastSFX.LoadSynchronous())
            ActiveVisualObjects.Add(Loaded);
    }
}

void UAetherSkillVisualComponent::ClearSkillVisuals()
{
    ActiveVisualObjects.Reset();
}
