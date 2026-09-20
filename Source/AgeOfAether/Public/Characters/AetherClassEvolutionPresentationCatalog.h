#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Characters/AetherClassEvolutionPresentationTypes.h"
#include "AetherClassEvolutionPresentationCatalog.generated.h"
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherClassEvolutionPresentationCatalog : public UDataAsset
{
 GENERATED_BODY()
public:
 UAetherClassEvolutionPresentationCatalog();
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TArray<FAetherClassEvolutionPresentationDefinition> Entries;
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") const FAetherClassEvolutionPresentationDefinition* Find(FName ClassID,FName EvolutionID) const;
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") const FAetherClassEvolutionPresentationDefinition* FindWithFallback(FName ClassID,FName EvolutionID) const;
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") bool ValidateCatalog(FString& OutError) const;
private: void BuildCanonicalCatalog();
};