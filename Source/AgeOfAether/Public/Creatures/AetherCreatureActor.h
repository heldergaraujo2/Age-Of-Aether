#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Creatures/AetherCreatureTypes.h"
#include "AetherCreatureActor.generated.h"

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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Creature")
    TObjectPtr<USkeletalMeshComponent> CreatureMesh;

    UPROPERTY(BlueprintReadOnly, Category="Creature")
    FAetherCreatureRuntimeState RuntimeState;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UObject>> PresentationAssets;

private:
    void ApplyPresentation(const FAetherCreatureDefinition& Definition);
};
