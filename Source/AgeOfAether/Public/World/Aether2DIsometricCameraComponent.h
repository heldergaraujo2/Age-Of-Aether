#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/Aether2DIsometricCameraProfile.h"
#include "Aether2DIsometricCameraComponent.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAether2DIsometricCameraComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAether2DIsometricCameraComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    bool ApplyProfileAsset(UAether2DIsometricCameraProfile* InProfile);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    void AddZoomInput(float AxisValue);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    UAether2DIsometricCameraProfile* GetProfile() const { return Profile; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    TObjectPtr<UAether2DIsometricCameraProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    bool bApplyOnBeginPlay = true;

private:
    bool ApplyLoadedProfile(UAether2DIsometricCameraProfile* InProfile);
    USpringArmComponent* ResolveCameraBoom() const;
    void ApplyCameraPolicy();
    float CurrentDistance = 0.0f;
};
