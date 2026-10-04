#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Creatures/AetherCreatureTypes.h"
#include "AetherCreatureActor.generated.h"

class USkeletalMeshComponent;
class UAether2DLivingVisualComponent;

UCLASS()
class AGEOFAETHER_API AAetherCreatureActor : public AActor
{
    GENERATED_BODY()

public:
    AAetherCreatureActor();

    bool ApplyDefinition(const FAetherCreatureDefinition& Definition);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Creature")
    const FAetherCreatureRuntimeState& GetRuntimeState() const { return RuntimeState; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Creature")
    FString GetCreatureID() const { return RuntimeState.CreatureID; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Creature")
    bool IsAlive() const { return RuntimeState.bAlive; }

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Creature")
    void ResetHealth();

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Creature|Combat")
    bool ApplyCombatDamage(float Damage);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Creature")
    TObjectPtr<USkeletalMeshComponent> CreatureMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Creature|2D")
    TObjectPtr<UAether2DLivingVisualComponent> Visual2DComponent;

    UPROPERTY(BlueprintReadOnly, Category="Creature")
    FAetherCreatureRuntimeState RuntimeState;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UObject>> PresentationAssets;

    UPROPERTY(Transient)
    FAetherCreatureDefinition DefinitionSnapshot;

private:
    void ApplyPresentation(const FAetherCreatureDefinition& Definition);
};
