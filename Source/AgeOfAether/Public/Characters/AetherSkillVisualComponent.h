#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skills/AetherSkillTypes.h"
#include "AetherSkillVisualComponent.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FAetherSkillVisualProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill Visual")
    FString SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill Visual")
    TSoftObjectPtr<UObject> CastVFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill Visual")
    TSoftObjectPtr<UObject> ImpactVFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill Visual")
    TSoftObjectPtr<UObject> CastSFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill Visual")
    TSoftObjectPtr<UAnimMontage> CastMontage;

    bool IsValid() const { return !SkillID.TrimStartAndEnd().IsEmpty(); }
};

UCLASS(ClassGroup=(AgeOfAether), meta=(BlueprintSpawnableComponent))
class AGEOFAETHER_API UAetherSkillVisualComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    void PlaySkillPresentation(const FAetherSkillVisualProfile& Profile, bool bImpact = false);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Skills")
    void ClearSkillVisuals();

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<UObject>> ActiveVisualObjects;
};
