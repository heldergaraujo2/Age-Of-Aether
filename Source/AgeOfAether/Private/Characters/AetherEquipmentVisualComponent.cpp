#include "Characters/AetherEquipmentVisualComponent.h"
#include "Characters/AetherCharacter.h"
#include "Characters/AetherEquipmentVisualProfile.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
UAetherEquipmentVisualComponent::UAetherEquipmentVisualComponent() { PrimaryComponentTick.bCanEverTick = false; }
void UAetherEquipmentVisualComponent::BeginPlay() { Super::BeginPlay(); }
bool UAetherEquipmentVisualComponent::ApplyEquipmentVisual(UAetherEquipmentVisualProfile* Profile)
{
    if (!Profile || GetNetMode() == NM_DedicatedServer) return false;
    FString Error; if (!Profile->ValidateProfile(Error)) { UE_LOG(LogTemp, Warning, TEXT("Aether equipment visual rejected: %s"), *Error); return false; }
    RemoveEquipmentVisual(Profile->EquipmentSlot);
    AAetherCharacter* Character = Cast<AAetherCharacter>(GetOwner());
    USkeletalMeshComponent* CharacterMesh = Character ? Character->GetMesh() : nullptr;
    if (!CharacterMesh) return false;
    UMeshComponent* Visual = nullptr;
    if (Profile->VisualType == EAetherEquipmentVisualType::SkeletalMesh)
    {
        USkeletalMeshComponent* Mesh = NewObject<USkeletalMeshComponent>(GetOwner());
        Mesh->SetSkeletalMesh(Profile->SkeletalMesh.LoadSynchronous()); if (!Mesh->GetSkeletalMeshAsset()) return false; Visual = Mesh;
    }
    else if (Profile->VisualType == EAetherEquipmentVisualType::StaticMesh)
    {
        UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(GetOwner());
        Mesh->SetStaticMesh(Profile->StaticMesh.LoadSynchronous()); if (!Mesh->GetStaticMesh()) return false; Visual = Mesh;
    }
    if (!Visual || !AttachMesh(Visual, Profile)) { if (Visual) Visual->DestroyComponent(); return false; }
    for (int32 Index=0; Index<Profile->MaterialOverrides.Num(); ++Index)
        if (UMaterialInterface* Material=Profile->MaterialOverrides[Index].LoadSynchronous()) Visual->SetMaterial(Index, Material);
    ActiveVisuals.Add(Profile->EquipmentSlot, Visual);
    return true;
}
bool UAetherEquipmentVisualComponent::AttachMesh(UMeshComponent* MeshComponent, const UAetherEquipmentVisualProfile* Profile)
{
    AAetherCharacter* Character=Cast<AAetherCharacter>(GetOwner());
    USkeletalMeshComponent* CharacterMesh=Character?Character->GetMesh():nullptr;
    if(!CharacterMesh||!MeshComponent||!Profile) return false;
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetGenerateOverlapEvents(false);
    MeshComponent->RegisterComponent();
    if(!Profile->AttachSocket.IsNone() && !CharacterMesh->DoesSocketExist(Profile->AttachSocket))
    { UE_LOG(LogTemp,Warning,TEXT("Aether equipment socket '%s' does not exist."),*Profile->AttachSocket.ToString()); MeshComponent->DestroyComponent(); return false; }
    const FName Socket=Profile->AttachSocket.IsNone()?CharacterMesh->GetBoneName(0):Profile->AttachSocket;
    MeshComponent->AttachToComponent(CharacterMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale,Socket);
    MeshComponent->SetRelativeTransform(Profile->RelativeTransform);
    return true;
}
void UAetherEquipmentVisualComponent::RemoveEquipmentVisual(EAetherEquipmentSlot Slot)
{
    if(TObjectPtr<UMeshComponent>* Existing=ActiveVisuals.Find(Slot)){ if(*Existing)(*Existing)->DestroyComponent(); ActiveVisuals.Remove(Slot); }
}
void UAetherEquipmentVisualComponent::ClearAllEquipmentVisuals()
{
    TArray<EAetherEquipmentSlot> Slots; ActiveVisuals.GetKeys(Slots); for(const EAetherEquipmentSlot Slot:Slots) RemoveEquipmentVisual(Slot);
}
UMeshComponent* UAetherEquipmentVisualComponent::GetEquipmentVisual(EAetherEquipmentSlot Slot) const
{
    const TObjectPtr<UMeshComponent>* Existing=ActiveVisuals.Find(Slot); return Existing?Existing->Get():nullptr;
}