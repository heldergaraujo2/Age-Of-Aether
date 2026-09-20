#include "Characters/AetherCharacter.h"

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
        Movement->MaxWalkSpeed = FoundationWalkSpeed;
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

    MoveForwardAction->ValueType = EInputActionValueType::Axis1D;
    MoveRightAction->ValueType = EInputActionValueType::Axis1D;
    LookYawAction->ValueType = EInputActionValueType::Axis1D;
    LookPitchAction->ValueType = EInputActionValueType::Axis1D;
    JumpAction->ValueType = EInputActionValueType::Boolean;

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
    AddControllerPitchInput(Value.Get<float>() * CameraTurnRate);
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
