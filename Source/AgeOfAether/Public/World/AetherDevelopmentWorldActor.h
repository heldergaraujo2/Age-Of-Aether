#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "GameFramework/Actor.h"

#include "AetherDevelopmentWorldActor.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;
class UStaticMesh;
class UStaticMeshComponent;
class UPaperSpriteComponent;
class UPaperSprite;
class UTexture2D;

UCLASS()
class AGEOFAETHER_API AAetherDevelopmentWorldActor : public AActor
{
    GENERATED_BODY()

public:
    AAetherDevelopmentWorldActor();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    TObjectPtr<UStaticMeshComponent> Ground;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    FVector GroundScale = FVector(24.0, 18.0, 0.20);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Foundation")
    float GroundZ = -10.0f;

private:
    void ConfigureGround();
    void BuildFirstRegionDiorama();
    UPaperSpriteComponent* AddSpriteArt(
        UPaperSprite* Sprite,
        const FName& Name,
        const FVector& Location,
        float Scale,
        float Yaw);
    UStaticMeshComponent* AddPrimitive(
        UStaticMesh* Mesh,
        const FName& Name,
        const FVector& Location,
        const FVector& Scale,
        const FLinearColor& Color,
        bool bBlockMovement = false);
    UMaterialInstanceDynamic* CreateColorMaterial(const FLinearColor& Color) const;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInterface> RuntimeBaseMaterial;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UStaticMeshComponent>> RuntimeVisualComponents;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UPaperSpriteComponent>> RuntimeSpriteComponents;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UPaperSprite>> RuntimeSprites;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UTexture2D>> RuntimeSpriteTextures;

    bool bDioramaBuilt = false;
};