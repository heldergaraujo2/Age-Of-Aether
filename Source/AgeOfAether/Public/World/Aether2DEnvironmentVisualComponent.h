#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/Aether2DEnvironmentVisualProfile.h"
#include "Aether2DEnvironmentVisualComponent.generated.h"

class UPaperSpriteComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAether2DEnvironmentVisualComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAether2DEnvironmentVisualComponent();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Environment")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Environment")
    bool ApplyProfileAsset(UAether2DEnvironmentVisualProfile* InProfile);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Environment")
    UPaperSpriteComponent* GetSpriteComponent() const { return SpriteComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Environment")
    UAether2DEnvironmentVisualProfile* GetProfile() const { return Profile; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    TObjectPtr<UAether2DEnvironmentVisualProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    bool bApplyOnBeginPlay = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Environment")
    TObjectPtr<UPaperSpriteComponent> SpriteComponent;

private:
    bool ApplyLoadedProfile(UAether2DEnvironmentVisualProfile* InProfile);
};
