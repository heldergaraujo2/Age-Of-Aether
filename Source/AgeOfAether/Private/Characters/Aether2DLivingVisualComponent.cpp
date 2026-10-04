#include "Characters/Aether2DLivingVisualComponent.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/PrimitiveComponent.h"

UAether2DLivingVisualComponent::UAether2DLivingVisualComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UAether2DLivingVisualComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bApplyOnBeginPlay && Profile)
    {
        ApplyProfile();
    }
}

void UAether2DLivingVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!FlipbookComponent || !Profile)
    {
        return;
    }

    if (CurrentState == EAether2DCharacterVisualState::Attack ||
        CurrentState == EAether2DCharacterVisualState::Hit ||
        CurrentState == EAether2DCharacterVisualState::Death ||
        CurrentState == EAether2DCharacterVisualState::Cast ||
        CurrentState == EAether2DCharacterVisualState::Interaction)
    {
        if (CurrentState != EAether2DCharacterVisualState::Death && !FlipbookComponent->IsPlaying())
        {
            SetVisualState(EAether2DCharacterVisualState::Idle, true);
        }
        return;
    }

    if (Profile->bDriveStateFromMovement)
    {
        UpdateMovementState();
    }
}

bool UAether2DLivingVisualComponent::ApplyProfile()
{
    if (!Profile || GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    return ApplyLoadedProfile(Profile);
}

bool UAether2DLivingVisualComponent::ApplyProfileAsset(UAether2DLivingVisualProfile* InProfile)
{
    if (!InProfile)
    {
        return false;
    }

    Profile = InProfile;
    return ApplyProfile();
}

bool UAether2DLivingVisualComponent::SetVisualState(EAether2DCharacterVisualState State, bool bLoop)
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

bool UAether2DLivingVisualComponent::ApplyLoadedProfile(UAether2DLivingVisualProfile* InProfile)
{
    FString ValidationError;
    if (!InProfile->ValidateProfile(ValidationError))
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether 2D living visual profile rejected: %s"), *ValidationError);
        return false;
    }

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return false;
    }

    if (!FlipbookComponent)
    {
        FlipbookComponent = NewObject<UPaperFlipbookComponent>(Owner, TEXT("Aether2DLivingFlipbook"));
        FlipbookComponent->RegisterComponent();
        FlipbookComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
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
        TArray<UActorComponent*> PrimitiveComponents;
        Owner->GetComponents(PrimitiveComponents);

        for (UActorComponent* Component : PrimitiveComponents)
        {
            if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component))
            {
                if (Primitive != FlipbookComponent && Primitive->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
                {
                    Primitive->SetVisibility(false, true);
                }
            }
        }
    }

    return true;
}

void UAether2DLivingVisualComponent::UpdateMovementState()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    const float Speed2D = Owner->GetVelocity().Size2D();
    const EAether2DCharacterVisualState Desired =
        Speed2D < 5.0f ? EAether2DCharacterVisualState::Idle :
        Speed2D > 500.0f ? EAether2DCharacterVisualState::Run :
        EAether2DCharacterVisualState::Walk;

    if (Desired != CurrentState && Profile->HasState(Desired))
    {
        SetVisualState(Desired, true);
    }
}
