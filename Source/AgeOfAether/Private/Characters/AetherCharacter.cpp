#include "Characters/AetherCharacter.h"
#include "Characters/AetherPlayableCharacterVisualComponent.h"
#include "Characters/AetherMovementCameraProfile.h"

#include "Characters/AetherCharacterPlayerState.h"
#include "Characters/AetherCharacterSubsystem.h"
#include "Networking/AetherNetworkPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "InputActionValue.h"
#include "InputModifiers.h"
#include "InputMappingContext.h"
#include "Net/UnrealNetwork.h"

AAetherCharacter::AAetherCharacter()
{
    bReplicates = true;
    SetReplicateMovement(true);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = FoundationWalkSpeed;
    GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = CameraDistance;
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, CameraHeight));
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bDoCollisionTest = true;
    CameraBoom->ProbeSize = 12.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    VisualComponent = CreateDefaultSubobject<UAetherPlayableCharacterVisualComponent>(TEXT("VisualComponent"));
}

void AAetherCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AAetherCharacter, CharacterId);
}

void AAetherCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!HasAuthority())
    {
        return;
    }

    AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>();
    AAetherNetworkPlayerController* CharacterController = Cast<AAetherNetworkPlayerController>(NewController);

    if (CharacterState && CharacterController && CharacterController->IsAccountAuthenticated())
    {
        CharacterId = CharacterState->GetCharacterId();
    }
}

void AAetherCharacter::UnPossessed()
{
    AController* PreviousController = GetController();

    if (HasAuthority())
    {
        if (AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>())
        {
            if (AAetherNetworkPlayerController* Controller = Cast<AAetherNetworkPlayerController>(PreviousController))
            {
                if (Controller->IsAccountAuthenticated())
                {
                    if (UAetherCharacterSubsystem* Characters = GetGameInstance()
                        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
                        : nullptr)
                    {
                        Characters->DeselectCharacter(
                            Controller->GetAuthenticatedAccountId(),
                            CharacterState->GetCharacterId());
                    }
                }
            }
        }
    }

    Super::UnPossessed();
}

void AAetherCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->MaxWalkSpeed = MovementCameraProfile ? MovementCameraProfile->WalkSpeed : FoundationWalkSpeed;
        Movement->JumpZVelocity = MovementCameraProfile ? MovementCameraProfile->JumpVelocity : Movement->JumpZVelocity;
        Movement->RotationRate = FRotator(0.0f, MovementCameraProfile ? MovementCameraProfile->RotationRate : 720.0f, 0.0f);
    }

    if (IsLocallyControlled())
    {
        InitializeFoundationInput();
    }
}

void AAetherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    InitializeFoundationInput();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AAetherCharacter::MoveForward);
        EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AAetherCharacter::MoveRight);
        EnhancedInput->BindAction(LookYawAction, ETriggerEvent::Triggered, this, &AAetherCharacter::LookYaw);
        EnhancedInput->BindAction(LookPitchAction, ETriggerEvent::Triggered, this, &AAetherCharacter::LookPitch);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AAetherCharacter::JumpPressed);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AAetherCharacter::SprintStarted);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAetherCharacter::SprintStopped);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AAetherCharacter::SprintStopped);
        EnhancedInput->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AAetherCharacter::CameraZoom);
    }
}

void AAetherCharacter::InitializeFoundationInput()
{
    if (!IsLocallyControlled() || RuntimeInputContext)
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !PC->GetLocalPlayer())
    {
        return;
    }

    RuntimeInputContext = NewObject<UInputMappingContext>(this, TEXT("AetherFoundationInput"));
    MoveForwardAction = NewObject<UInputAction>(this, TEXT("MoveForward"));
    MoveRightAction = NewObject<UInputAction>(this, TEXT("MoveRight"));
    LookYawAction = NewObject<UInputAction>(this, TEXT("LookYaw"));
    LookPitchAction = NewObject<UInputAction>(this, TEXT("LookPitch"));
    JumpAction = NewObject<UInputAction>(this, TEXT("Jump"));
    SprintAction = NewObject<UInputAction>(this, TEXT("Sprint"));
    CameraZoomAction = NewObject<UInputAction>(this, TEXT("CameraZoom"));

    MoveForwardAction->ValueType = EInputActionValueType::Axis1D;
    MoveRightAction->ValueType = EInputActionValueType::Axis1D;
    LookYawAction->ValueType = EInputActionValueType::Axis1D;
    LookPitchAction->ValueType = EInputActionValueType::Axis1D;
    JumpAction->ValueType = EInputActionValueType::Boolean;
    SprintAction->ValueType = EInputActionValueType::Boolean;
    CameraZoomAction->ValueType = EInputActionValueType::Axis1D;

    RuntimeInputContext->MapKey(MoveForwardAction, EKeys::W);
    {
        FEnhancedActionKeyMapping& Mapping = RuntimeInputContext->MapKey(MoveForwardAction, EKeys::S);
        Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(RuntimeInputContext));
    }
    RuntimeInputContext->MapKey(MoveRightAction, EKeys::D);
    {
        FEnhancedActionKeyMapping& Mapping = RuntimeInputContext->MapKey(MoveRightAction, EKeys::A);
        Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(RuntimeInputContext));
    }
    RuntimeInputContext->MapKey(LookYawAction, EKeys::MouseX);
    RuntimeInputContext->MapKey(LookPitchAction, EKeys::MouseY);
    RuntimeInputContext->MapKey(JumpAction, EKeys::SpaceBar);
    RuntimeInputContext->MapKey(SprintAction, EKeys::LeftShift);
    RuntimeInputContext->MapKey(CameraZoomAction, EKeys::MouseWheelAxis);

    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
        InputSubsystem->AddMappingContext(RuntimeInputContext, 0);
    }
}

void AAetherCharacter::MoveForward(const FInputActionValue& Value)
{
    if (!Controller)
    {
        return;
    }

    const float Axis = Value.Get<float>();
    const FRotator ControlRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X), Axis);
}

void AAetherCharacter::MoveRight(const FInputActionValue& Value)
{
    if (!Controller)
    {
        return;
    }

    const float Axis = Value.Get<float>();
    const FRotator ControlRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y), Axis);
}

void AAetherCharacter::LookYaw(const FInputActionValue& Value)
{
    AddControllerYawInput(Value.Get<float>() * CameraTurnRate);
}

void AAetherCharacter::LookPitch(const FInputActionValue& Value)
{
    const float Input = Value.Get<float>() * CameraTurnRate;
    const float MinPitch = MovementCameraProfile ? MovementCameraProfile->CameraMinPitch : -75.0f;
    const float MaxPitch = MovementCameraProfile ? MovementCameraProfile->CameraMaxPitch : 35.0f;
    const float CurrentPitch = FRotator::NormalizeAxis(GetControlRotation().Pitch);
    const float NewPitch = FMath::Clamp(CurrentPitch + Input, MinPitch, MaxPitch);
    AddControllerPitchInput(NewPitch - CurrentPitch);
}

void AAetherCharacter::JumpPressed(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        Jump();
    }
}

void AAetherCharacter::InitializeCharacterIdentity(const FAetherCharacterId& InCharacterId)
{
    if (HasAuthority())
    {
        CharacterId = InCharacterId;
    }
}

FAetherCharacterId AAetherCharacter::GetCharacterId() const
{
    return CharacterId;
}

void AAetherCharacter::SprintStarted(const FInputActionValue& Value)
{
    if (!Value.Get<bool>()) return;
    bSprinting = true;
    if (!HasAuthority()) ServerSetSprinting(true);
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
        Movement->MaxWalkSpeed = MovementCameraProfile ? MovementCameraProfile->SprintSpeed : FoundationWalkSpeed * 1.5f;
}

void AAetherCharacter::SprintStopped(const FInputActionValue& Value)
{
    bSprinting = false;
    if (!HasAuthority()) ServerSetSprinting(false);
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
        Movement->MaxWalkSpeed = MovementCameraProfile ? MovementCameraProfile->WalkSpeed : FoundationWalkSpeed;
}

void AAetherCharacter::CameraZoom(const FInputActionValue& Value)
{
    if (!CameraBoom) return;
    const float Step = MovementCameraProfile ? MovementCameraProfile->CameraZoomStep : 50.0f;
    const float MinDistance = MovementCameraProfile ? MovementCameraProfile->MinCameraDistance : 250.0f;
    const float MaxDistance = MovementCameraProfile ? MovementCameraProfile->MaxCameraDistance : 650.0f;
    CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - Value.Get<float>() * Step, MinDistance, MaxDistance);
}

void AAetherCharacter::ServerSetSprinting_Implementation(bool bNewSprinting)
{
    if (!HasAuthority() || !GetController()) return;
    bSprinting = bNewSprinting;
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        const float Walk = MovementCameraProfile ? MovementCameraProfile->WalkSpeed : FoundationWalkSpeed;
        const float Sprint = MovementCameraProfile ? MovementCameraProfile->SprintSpeed : FoundationWalkSpeed * 1.5f;
        Movement->MaxWalkSpeed = bSprinting ? Sprint : Walk;
    }
}
