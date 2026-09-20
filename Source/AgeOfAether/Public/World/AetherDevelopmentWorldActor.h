#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AetherDevelopmentWorldActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class AGEOFAETHER_API AAetherDevelopmentWorldActor : public AActor
{
    GENERATED_BODY()

public:
    AAetherDevelopmentWorldActor();

    virtual void OnConstruction(const FTransform& Transform) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<UStaticMeshComponent> Ground;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    FVector GroundScale = FVector(25.0, 25.0, 0.25);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    float GroundZ = -12.5f;

private:
    void ConfigureGround();
};
