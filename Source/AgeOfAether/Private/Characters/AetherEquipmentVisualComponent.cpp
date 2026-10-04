#include "Characters/AetherEquipmentVisualComponent.h"
#include "Characters/AetherCharacter.h"
#include "Characters/AetherEquipmentVisualProfile.h"
#include "Characters/Aether2DCharacterVisualComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
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
    UMeshComponent* Visual = nullptr;
    if (Profile->VisualType == EAetherEquipmentVisualType::PaperSprite)
    {
        UPaperSpriteComponent* SpriteComponent = NewObject<UPaperSpriteComponent>(GetOwner());
        SpriteComponent->SetSprite(Profile->Sprite.LoadSynchronous());
        if (!SpriteComponent->GetSprite()) return false;
        SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SpriteComponent->SetGenerateOverlapEvents(false);
        SpriteComponent->SetRelativeLocation(Profile->SpriteWorldOffset);
        SpriteComponent->SetRelativeScale3D(FVector(Profile->SpriteScale.X, Profile->SpriteScale.Y, 1.0f));
        SpriteComponent->TranslucencySortPriority = Profile->RenderLayer;
        if (UAether2DCharacterVisualComponent* Visual2D = Character ? Character->Get2DVisualComponent() : nullptr)
        {
            if (UPaperFlipbookComponent* Flipbook = Visual2D->GetFlipbookComponent())
            {
                SpriteComponent->RegisterComponent();
                SpriteComponent->AttachToComponent(Flipbook, FAttachmentTransformRules::KeepRelativeTransform);
                Visual = SpriteComponent;
            }
        }
        if (!Visual) { SpriteComponent->DestroyComponent(); return false; }
    }
    else if (Profile->VisualType == EAetherEquipmentVisualType::SkeletalMesh)
    {
        if (!CharacterMesh) return false;
        USkeletalMeshComponent* Mesh = NewObject<USkeletalMeshComponent>(GetOwner());
        Mesh->SetSkeletalMesh(Profile->SkeletalMesh.LoadSynchronous()); if (!Mesh->GetSkeletalMeshAsset()) return false; Visual = Mesh;
    }
    else if (Profile->VisualType == EAetherEquipmentVisualType::StaticMesh)
    {
        if (!CharacterMesh) return false;
        UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(GetOwner());
        Mesh->SetStaticMesh(Profile->StaticMesh.LoadSynchronous()); if (!Mesh->GetStaticMesh()) return false; Visual = Mesh;
    }
    if (!Visual) return false;
    if (Profile->VisualType != EAetherEquipmentVisualType::PaperSprite && !AttachMesh(Visual, Profile)) { if (Visual) Visual->DestroyComponent(); return false; }
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
void UAetherEquipmentVisualComponent::RemoveEquipmentVisual(EAetherDataEquipmentSlot Slot)
{
    if(TObjectPtr<UMeshComponent>* Existing=ActiveVisuals.Find(Slot)){ if(*Existing)(*Existing)->DestroyComponent(); ActiveVisuals.Remove(Slot); }
}
void UAetherEquipmentVisualComponent::ClearAllEquipmentVisuals()
{
    TArray<EAetherDataEquipmentSlot> Slots; ActiveVisuals.GetKeys(Slots); for(const EAetherDataEquipmentSlot Slot:Slots) RemoveEquipmentVisual(Slot);
}
UMeshComponent* UAetherEquipmentVisualComponent::GetEquipmentVisual(EAetherDataEquipmentSlot Slot) const
{
    const TObjectPtr<UMeshComponent>* Existing=ActiveVisuals.Find(Slot); return Existing?Existing->Get():nullptr;
}