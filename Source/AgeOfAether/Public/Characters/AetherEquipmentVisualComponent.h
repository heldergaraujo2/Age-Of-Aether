#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/AetherItemTypes.h"
#include "AetherEquipmentVisualComponent.generated.h"
class UAetherEquipmentVisualProfile; class UMeshComponent;
UCLASS(ClassGroup=(AgeOfAether), meta=(BlueprintSpawnableComponent))
class AGEOFAETHER_API UAetherEquipmentVisualComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UAetherEquipmentVisualComponent();
    UFUNCTION(BlueprintCallable, Category="Age of Aether|Equipment") bool ApplyEquipmentVisual(UAetherEquipmentVisualProfile* Profile);
    UFUNCTION(BlueprintCallable, Category="Age of Aether|Equipment") void RemoveEquipmentVisual(EAetherEquipmentSlot Slot);
    UFUNCTION(BlueprintCallable, Category="Age of Aether|Equipment") void ClearAllEquipmentVisuals();
    UFUNCTION(BlueprintPure, Category="Age of Aether|Equipment") UMeshComponent* GetEquipmentVisual(EAetherEquipmentSlot Slot) const;
protected: virtual void BeginPlay() override;
private:
    UPROPERTY(Transient) TMap<EAetherEquipmentSlot, TObjectPtr<UMeshComponent>> ActiveVisuals;
    bool AttachMesh(UMeshComponent* MeshComponent, const UAetherEquipmentVisualProfile* Profile);
};