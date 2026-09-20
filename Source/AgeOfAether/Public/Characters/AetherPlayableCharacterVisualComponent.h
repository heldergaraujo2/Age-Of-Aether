#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AetherPlayableCharacterVisualComponent.generated.h"

class UAetherPlayableCharacterVisualProfile;
class USkeletalMeshComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAetherPlayableCharacterVisualComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAetherPlayableCharacterVisualComponent();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Visual")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Visual")
    bool ApplyProfileAsset(UAetherPlayableCharacterVisualProfile* InProfile);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Animation")
    bool ApplyAnimationProfile(class UAetherCharacterAnimationProfile* InAnimationProfile);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    UAetherPlayableCharacterVisualProfile* GetProfile() const { return Profile; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    USkeletalMeshComponent* GetMeshComponent() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<UAetherPlayableCharacterVisualProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    bool bApplyOnBeginPlay = true;

private:
    bool ApplyLoadedProfile(UAetherPlayableCharacterVisualProfile* InProfile);
};
