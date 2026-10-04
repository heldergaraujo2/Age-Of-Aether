#include "Characters/AetherSkillVisualComponent.h"
#include "Characters/AetherCharacter.h"
#include "Characters/Aether2DCharacterVisualComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"

void UAetherSkillVisualComponent::PlaySkillPresentation(const FAetherSkillVisualProfile& Profile, bool bImpact)
{
    if (GetOwner() && GetOwner()->GetNetMode() == NM_DedicatedServer) return;
    if (!Profile.IsValid()) return;

    if (!bImpact)
    {
        if (AAetherCharacter* Character = Cast<AAetherCharacter>(GetOwner()))
        {
            if (UAether2DCharacterVisualComponent* Visual2D = Character->Get2DVisualComponent())
            {
                Visual2D->SetVisualState(EAether2DCharacterVisualState::Cast, false);
            }
        }
    }

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
