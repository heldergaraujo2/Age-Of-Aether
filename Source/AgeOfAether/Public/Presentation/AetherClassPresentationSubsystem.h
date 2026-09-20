#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Presentation/AetherClassPresentationTypes.h"
#include "AetherClassPresentationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAetherClassPresentationChangedEvent,FString,ClassID,FString,EvolutionID,EAetherClassPresentationLoadState,State);

UCLASS()
class AGEOFAETHER_API UAetherClassPresentationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable,Category="Age of Aether|Class Presentation") void ResetClassPresentation();
    UFUNCTION(BlueprintCallable,Category="Age of Aether|Class Presentation") void SetPresentation(const FAetherClassPresentationDefinition& Definition);
    UFUNCTION(BlueprintCallable,Category="Age of Aether|Class Presentation") bool ResolvePresentation(const FString& ClassID,const FString& EvolutionID,FAetherClassPresentationBinding& OutBinding) const;
    UFUNCTION(BlueprintCallable,Category="Age of Aether|Class Presentation") void SetLoadState(const FString& ClassID,const FString& EvolutionID,EAetherClassPresentationLoadState State);
    UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") EAetherClassPresentationLoadState GetLoadState(const FString& ClassID,const FString& EvolutionID) const;
    UPROPERTY(BlueprintAssignable,Category="Age of Aether|Class Presentation") FAetherClassPresentationChangedEvent OnPresentationChanged;
private:
    TMap<FString,FAetherClassPresentationDefinition> Definitions;
    TMap<FString,EAetherClassPresentationLoadState> States;
    static FString Key(const FString& ClassID,const FString& EvolutionID);
};
