#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AetherClassEvolutionPresentationComponent.generated.h"
class UAetherClassEvolutionPresentationCatalog;
UCLASS(ClassGroup=(AgeOfAether),BlueprintType,Blueprintable,meta=(BlueprintSpawnableComponent))
class AGEOFAETHER_API UAetherClassEvolutionPresentationComponent : public UActorComponent
{
 GENERATED_BODY()
public:
 UAetherClassEvolutionPresentationComponent();
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Class Presentation") bool ApplyClassEvolution(FName ClassID,FName EvolutionID);
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") FName GetAppliedClassID() const{return AppliedClassID;}
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") FName GetAppliedEvolutionID() const{return AppliedEvolutionID;}
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") bool IsUsingFallback() const{return bUsingFallback;}
 UFUNCTION(BlueprintPure,Category="Age of Aether|Class Presentation") UAetherClassEvolutionPresentationCatalog* GetCatalog() const{return Catalog;}
protected:
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TObjectPtr<UAetherClassEvolutionPresentationCatalog> Catalog;
 UPROPERTY(Transient,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FName AppliedClassID;
 UPROPERTY(Transient,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FName AppliedEvolutionID;
 UPROPERTY(Transient,BlueprintReadOnly,Category="Age of Aether|Class Presentation") bool bUsingFallback=false;
private:
 UAetherClassEvolutionPresentationCatalog* GetOrCreateCatalog();
};