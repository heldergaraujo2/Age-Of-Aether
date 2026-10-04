#include "Characters/Aether2DCharacterVisualComponent.h"

#include "Characters/AetherCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

UAether2DCharacterVisualComponent::UAether2DCharacterVisualComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UAether2DCharacterVisualComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bApplyOnBeginPlay && Profile)
    {
        ApplyProfile();
    }
}

void UAether2DCharacterVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bDriveStateFromMovement && FlipbookComponent && CurrentState != EAether2DCharacterVisualState::Attack &&
        CurrentState != EAether2DCharacterVisualState::Hit && CurrentState != EAether2DCharacterVisualState::Death)
    {
        UpdateMovementState();
    }
}

bool UAether2DCharacterVisualComponent::ApplyProfile()
{
    if (!Profile || GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    return ApplyLoadedProfile(Profile);
}

bool UAether2DCharacterVisualComponent::ApplyProfileAsset(UAether2DCharacterVisualProfile* InProfile)
{
    if (!InProfile)
    {
        return false;
    }

    Profile = InProfile;
    return ApplyProfile();
}

bool UAether2DCharacterVisualComponent::SetVisualState(EAether2DCharacterVisualState State, bool bLoop)
{
    if (!Profile || !FlipbookComponent || !Profile->HasState(State) || GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    const TSoftObjectPtr<UPaperFlipbook>* FlipbookRef = Profile->Flipbooks.Find(State);
    if (!FlipbookRef)
    {
        return false;
    }

    UPaperFlipbook* Flipbook = FlipbookRef->LoadSynchronous();
    if (!Flipbook)
    {
        return false;
    }

    CurrentState = State;
    FlipbookComponent->SetLooping(bLoop);
    FlipbookComponent->SetFlipbook(Flipbook);
    FlipbookComponent->PlayFromStart();
    return true;
}

bool UAether2DCharacterVisualComponent::ApplyLoadedProfile(UAether2DCharacterVisualProfile* InProfile)
{
    FString ValidationError;
    if (!InProfile->ValidateProfile(ValidationError))
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether 2D visual profile rejected: %s"), *ValidationError);
        return false;
    }

    AAetherCharacter* Character = Cast<AAetherCharacter>(GetOwner());
    if (!Character)
    {
        return false;
    }

    if (!FlipbookComponent)
    {
        FlipbookComponent = NewObject<UPaperFlipbookComponent>(Character, TEXT("Aether2DFlipbook"));
        FlipbookComponent->RegisterComponent();
        FlipbookComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

    FlipbookComponent->SetRelativeLocation(InProfile->SpriteWorldOffset);
    FlipbookComponent->SetRelativeScale3D(FVector(InProfile->VisualScale.X, InProfile->VisualScale.Y, 1.0f));
    FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FlipbookComponent->SetCastShadow(false);

    if (!SetVisualState(EAether2DCharacterVisualState::Idle, true))
    {
        return false;
    }

    if (InProfile->bUseAsPrimaryPresentation)
    {
        if (USkeletalMeshComponent* Mesh = Character->GetMesh())
        {
            Mesh->SetVisibility(false, true);
        }
    }

    return true;
}

void UAether2DCharacterVisualComponent::UpdateMovementState()
{
    AAetherCharacter* Character = Cast<AAetherCharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    const float Speed2D = Character->GetVelocity().Size2D();
    const EAether2DCharacterVisualState Desired =
        Speed2D < 5.0f ? EAether2DCharacterVisualState::Idle :
        Speed2D > 500.0f ? EAether2DCharacterVisualState::Run :
        EAether2DCharacterVisualState::Walk;

    if (Desired != CurrentState && Profile->HasState(Desired))
    {
        SetVisualState(Desired, true);
    }
}

void UAether2DCharacterVisualComponent::RestorePreviousPresentationVisibility()
{
}
