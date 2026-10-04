#include "Creatures/AetherCreatureActor.h"
#include "Characters/Aether2DLivingVisualComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

AAetherCreatureActor::AAetherCreatureActor()
{
    PrimaryActorTick.bCanEverTick = false;
    CreatureMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CreatureMesh"));
    SetRootComponent(CreatureMesh);
    CreatureMesh->SetCollisionProfileName(TEXT("Pawn"));
    CreatureMesh->SetGenerateOverlapEvents(false);
    CreatureMesh->SetCanEverAffectNavigation(true);
    Visual2DComponent = CreateDefaultSubobject<UAether2DLivingVisualComponent>(TEXT("Visual2DComponent"));
}

bool AAetherCreatureActor::ApplyDefinition(const FAetherCreatureDefinition& Definition)
{
    DefinitionSnapshot = Definition;
    if (GetNetMode() == NM_DedicatedServer)
    {
        RuntimeState.CreatureID = Definition.CreatureID.TrimStartAndEnd().ToLower();
        RuntimeState.Role = Definition.Role;
        RuntimeState.MaxHealth = Definition.MaxHealth;
        RuntimeState.CurrentHealth = Definition.MaxHealth;
        RuntimeState.Level = Definition.Level;
        RuntimeState.bAlive = true;
        return Definition.IsValid();
    }

    if (!Definition.IsValid()) return false;

    RuntimeState.CreatureID = Definition.CreatureID.TrimStartAndEnd().ToLower();
    RuntimeState.Role = Definition.Role;
    RuntimeState.MaxHealth = Definition.MaxHealth;
    RuntimeState.CurrentHealth = Definition.MaxHealth;
    RuntimeState.Level = Definition.Level;
    RuntimeState.bAlive = true;
    ApplyPresentation(Definition);
    if (Visual2DComponent && Definition.Visual2DProfile)
    {
        Visual2DComponent->ApplyProfileAsset(Definition.Visual2DProfile);
    }
    return true;
}

void AAetherCreatureActor::ResetHealth()
{
    RuntimeState.CurrentHealth = RuntimeState.MaxHealth;
    RuntimeState.bAlive = RuntimeState.MaxHealth > 0;
    if (Visual2DComponent && RuntimeState.bAlive)
    {
        Visual2DComponent->SetVisualState(EAether2DCharacterVisualState::Idle, true);
    }
}

bool AAetherCreatureActor::ApplyCombatDamage(float Damage)
{
    if (!RuntimeState.bAlive || !FMath::IsFinite(Damage) || Damage < 0.0f)
    {
        return false;
    }

    RuntimeState.CurrentHealth = FMath::Max(0.0f, RuntimeState.CurrentHealth - Damage);
    if (RuntimeState.CurrentHealth <= 0.0f)
    {
        RuntimeState.CurrentHealth = 0.0f;
        RuntimeState.bAlive = false;
        if (Visual2DComponent)
        {
            Visual2DComponent->SetVisualState(EAether2DCharacterVisualState::Death, false);
        }
    }
    else if (Visual2DComponent)
    {
        Visual2DComponent->SetVisualState(EAether2DCharacterVisualState::Hit, false);
    }

    return true;
}

void AAetherCreatureActor::ApplyPresentation(const FAetherCreatureDefinition& Definition)
{
    if (Definition.SkeletalMesh.IsValid())
    {
        if (USkeletalMesh* Mesh = Definition.SkeletalMesh.LoadSynchronous())
        {
            CreatureMesh->SetSkeletalMesh(Mesh);
            if (Definition.AnimationClass.IsValid())
                CreatureMesh->SetAnimInstanceClass(Definition.AnimationClass.LoadSynchronous());
        }
    }

    if (Definition.IdleVFX.IsValid())
        if (UObject* Asset = Definition.IdleVFX.LoadSynchronous()) PresentationAssets.Add(Asset);
    if (Definition.SpawnVFX.IsValid())
        if (UObject* Asset = Definition.SpawnVFX.LoadSynchronous()) PresentationAssets.Add(Asset);
    if (Definition.SpawnSFX.IsValid())
        if (UObject* Asset = Definition.SpawnSFX.LoadSynchronous()) PresentationAssets.Add(Asset);
}

