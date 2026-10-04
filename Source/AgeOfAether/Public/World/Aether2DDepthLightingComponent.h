#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/Aether2DDepthLightingProfile.h"
#include "Aether2DDepthLightingComponent.generated.h"

class UPrimitiveComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAether2DDepthLightingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAether2DDepthLightingComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Depth")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Depth")
    bool ApplyProfileAsset(UAether2DDepthLightingProfile* InProfile);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Depth")
    UAether2DDepthLightingProfile* GetProfile() const { return Profile; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    TObjectPtr<UAether2DDepthLightingProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    bool bApplyOnBeginPlay = true;

private:
    bool ApplyLoadedProfile(UAether2DDepthLightingProfile* InProfile);
    void ApplyParallax();
    TObjectPtr<UPrimitiveComponent> ResolvePresentationComponent() const;
    FVector InitialOwnerLocation = FVector::ZeroVector;
    FVector InitialCameraLocation = FVector::ZeroVector;
};
