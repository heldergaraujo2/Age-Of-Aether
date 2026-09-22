#include "Creatures/AetherCreatureActor.h"
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
}

bool AAetherCreatureActor::ApplyDefinition(const FAetherCreatureDefinition& Definition)
{
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
    return true;
}

void AAetherCreatureActor::ResetHealth()
{
    RuntimeState.CurrentHealth = RuntimeState.MaxHealth;
    RuntimeState.bAlive = RuntimeState.MaxHealth > 0;
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
