#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/AetherUITypes.h"
#include "AetherHUDPresentationComponent.generated.h"
UCLASS(ClassGroup=(UI), meta=(BlueprintSpawnableComponent))
class AGEOFAETHER_API UAetherHUDPresentationComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UAetherHUDPresentationComponent();
    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI") void SetHUDVisible(bool bVisible);
    UFUNCTION(BlueprintPure, Category="Age of Aether|UI") bool IsHUDVisible() const { return bVisible; }
private:
    UPROPERTY(Transient) bool bVisible=true;
};