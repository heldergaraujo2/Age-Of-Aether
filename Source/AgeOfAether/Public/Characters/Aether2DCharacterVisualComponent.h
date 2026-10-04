#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Aether2DCharacterVisualProfile.h"
#include "Aether2DCharacterVisualComponent.generated.h"

class UPaperFlipbookComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAether2DCharacterVisualComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAether2DCharacterVisualComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Visual")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Visual")
    bool ApplyProfileAsset(UAether2DCharacterVisualProfile* InProfile);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Visual")
    bool SetVisualState(EAether2DCharacterVisualState State, bool bLoop = true);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Visual")
    EAether2DCharacterVisualState GetVisualState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Visual")
    UPaperFlipbookComponent* GetFlipbookComponent() const { return FlipbookComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Visual")
    UAether2DCharacterVisualProfile* GetProfile() const { return Profile; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    TObjectPtr<UAether2DCharacterVisualProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    bool bApplyOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    bool bDriveStateFromMovement = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;

private:
    bool ApplyLoadedProfile(UAether2DCharacterVisualProfile* InProfile);
    void UpdateMovementState();
    void RestorePreviousPresentationVisibility();
    EAether2DCharacterVisualState CurrentState = EAether2DCharacterVisualState::Idle;
};
