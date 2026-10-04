#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Aether2DLivingVisualProfile.h"
#include "Aether2DLivingVisualComponent.generated.h"

class UPaperFlipbookComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAether2DLivingVisualComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAether2DLivingVisualComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Living Visual")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Living Visual")
    bool ApplyProfileAsset(UAether2DLivingVisualProfile* InProfile);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Living Visual")
    bool SetVisualState(EAether2DCharacterVisualState State, bool bLoop = true);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Living Visual")
    EAether2DCharacterVisualState GetVisualState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Living Visual")
    UPaperFlipbookComponent* GetFlipbookComponent() const { return FlipbookComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Living Visual")
    UAether2DLivingVisualProfile* GetProfile() const { return Profile; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    TObjectPtr<UAether2DLivingVisualProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    bool bApplyOnBeginPlay = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Living Visual")
    TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;

private:
    bool ApplyLoadedProfile(UAether2DLivingVisualProfile* InProfile);
    void UpdateMovementState();
    EAether2DCharacterVisualState CurrentState = EAether2DCharacterVisualState::Idle;
};
