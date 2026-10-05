#include "Characters/AetherCharacter.h"
#include "Characters/AetherPlayableCharacterVisualComponent.h"
#include "Characters/Aether2DCharacterVisualComponent.h"
#include "World/Aether2DIsometricCameraComponent.h"
#include "Characters/AetherEquipmentVisualComponent.h"
#include "Characters/AetherClassEvolutionPresentationComponent.h"
#include "Characters/AetherSkillVisualComponent.h"
#include "Characters/AetherMovementCameraProfile.h"

#include "Characters/AetherCharacterPlayerState.h"
#include "Characters/AetherCharacterSubsystem.h"
#include "Networking/AetherNetworkPlayerController.h"
#include "GameFramework/PlayerController.h"

#include "Camera/CameraComponent.h"
#include "Animation/AnimSequence.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/SkeletalMesh.h"
#include "PaperSpriteComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

AAetherCharacter::AAetherCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);

    MageSkeletalMeshAsset = TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(
        TEXT("/Game/Aether/Characters/Mage/SK_Mago_AgeOfAether.SK_Mago_AgeOfAether")));
    MageIdleAnimationAsset = TSoftObjectPtr<UAnimSequence>(FSoftObjectPath(
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Idle.A_Idle")));
    MageWalkAnimationAsset = TSoftObjectPtr<UAnimSequence>(FSoftObjectPath(
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Walk.A_Walk")));
    MageRunAnimationAsset = TSoftObjectPtr<UAnimSequence>(FSoftObjectPath(
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Run.A_Run")));
    MageJumpAnimationAsset = TSoftObjectPtr<UAnimSequence>(FSoftObjectPath(
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Jump.A_Jump")));

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
    EquipmentVisualComponent = CreateDefaultSubobject<UAetherEquipmentVisualComponent>(TEXT("EquipmentVisualComponent"));
    ClassEvolutionPresentationComponent = CreateDefaultSubobject<UAetherClassEvolutionPresentationComponent>(TEXT("ClassEvolutionPresentationComponent"));
    SkillVisualComponent = CreateDefaultSubobject<UAetherSkillVisualComponent>(TEXT("SkillVisualComponent"));
    Visual2DComponent = CreateDefaultSubobject<UAether2DCharacterVisualComponent>(TEXT("Visual2DComponent"));
    IsometricCameraComponent = CreateDefaultSubobject<UAether2DIsometricCameraComponent>(TEXT("IsometricCameraComponent"));

    RuntimeSkeletalVisual = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RuntimeSkeletalVisual"));
    RuntimeSkeletalVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeSkeletalVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RuntimeSkeletalVisual->SetCastShadow(true);

    RuntimeBodyVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeBodyVisual"));
    RuntimeBodyVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeBodyVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeHeadVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeHeadVisual"));
    RuntimeHeadVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeHeadVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeMantleVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeMantleVisual"));
    RuntimeMantleVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeMantleVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeLeftArmVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeLeftArmVisual"));
    RuntimeLeftArmVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeLeftArmVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeRightArmVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeRightArmVisual"));
    RuntimeRightArmVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeRightArmVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeLeftHandVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeLeftHandVisual"));
    RuntimeLeftHandVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeLeftHandVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeRightHandVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeRightHandVisual"));
    RuntimeRightHandVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeRightHandVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeLeftLegVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeLeftLegVisual"));
    RuntimeLeftLegVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeLeftLegVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeRightLegVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeRightLegVisual"));
    RuntimeRightLegVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeRightLegVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeLeftBootVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeLeftBootVisual"));
    RuntimeLeftBootVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeLeftBootVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeRightBootVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeRightBootVisual"));
    RuntimeRightBootVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeRightBootVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeHairVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeHairVisual"));
    RuntimeHairVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeHairVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeHatVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeHatVisual"));
    RuntimeHatVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeHatVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeBeltVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeBeltVisual"));
    RuntimeBeltVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeBeltVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeStaffVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeStaffVisual"));
    RuntimeStaffVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeStaffVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RuntimeOrbVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RuntimeOrbVisual"));
    RuntimeOrbVisual->SetupAttachment(GetCapsuleComponent());
    RuntimeOrbVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Runtime2DArtVisual = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Runtime2DArtVisual"));
    Runtime2DArtVisual->SetupAttachment(GetCapsuleComponent());
    Runtime2DArtVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Runtime2DArtVisual->SetCastShadow(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    if (CylinderMesh.Succeeded())
    {
        RuntimeBodyVisual->SetStaticMesh(CylinderMesh.Object);
        RuntimeMantleVisual->SetStaticMesh(ConeMesh.Succeeded() ? ConeMesh.Object : CylinderMesh.Object);
        RuntimeLeftArmVisual->SetStaticMesh(CylinderMesh.Object);
        RuntimeRightArmVisual->SetStaticMesh(CylinderMesh.Object);
        RuntimeLeftLegVisual->SetStaticMesh(CylinderMesh.Object);
        RuntimeRightLegVisual->SetStaticMesh(CylinderMesh.Object);
        RuntimeBeltVisual->SetStaticMesh(CylinderMesh.Object);
        RuntimeStaffVisual->SetStaticMesh(CylinderMesh.Object);
    }
    if (SphereMesh.Succeeded())
    {
        RuntimeHeadVisual->SetStaticMesh(SphereMesh.Object);
        RuntimeLeftHandVisual->SetStaticMesh(SphereMesh.Object);
        RuntimeRightHandVisual->SetStaticMesh(SphereMesh.Object);
        RuntimeHairVisual->SetStaticMesh(SphereMesh.Object);
        RuntimeOrbVisual->SetStaticMesh(SphereMesh.Object);
    }
    if (ConeMesh.Succeeded())
    {
        RuntimeHatVisual->SetStaticMesh(ConeMesh.Object);
    }
    if (CubeMesh.Succeeded())
    {
        RuntimeLeftBootVisual->SetStaticMesh(CubeMesh.Object);
        RuntimeRightBootVisual->SetStaticMesh(CubeMesh.Object);
    }

    const auto ApplyColor = [this](UStaticMeshComponent* Component, const FLinearColor& Color)
    {
        if (BaseMaterial.Succeeded() && Component)
        {
            if (UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial.Object, this))
            {
                Material->SetVectorParameterValue(TEXT("Color"), Color);
                Material->SetScalarParameterValue(TEXT("Roughness"), 0.76f);
                Material->SetScalarParameterValue(TEXT("Specular"), 0.20f);
                Material->SetScalarParameterValue(TEXT("Metallic"), 0.0f);
                Component->SetMaterial(0, Material);
            }
        }
    };

    ApplyColor(RuntimeBodyVisual, FLinearColor(0.19f, 0.27f, 0.54f, 1.0f));
    ApplyColor(RuntimeHeadVisual, FLinearColor(0.82f, 0.58f, 0.42f, 1.0f));
    ApplyColor(RuntimeMantleVisual, FLinearColor(0.31f, 0.18f, 0.46f, 1.0f));
    ApplyColor(RuntimeLeftArmVisual, FLinearColor(0.31f, 0.18f, 0.46f, 1.0f));
    ApplyColor(RuntimeRightArmVisual, FLinearColor(0.31f, 0.18f, 0.46f, 1.0f));
    ApplyColor(RuntimeLeftHandVisual, FLinearColor(0.82f, 0.58f, 0.42f, 1.0f));
    ApplyColor(RuntimeRightHandVisual, FLinearColor(0.82f, 0.58f, 0.42f, 1.0f));
    ApplyColor(RuntimeLeftLegVisual, FLinearColor(0.16f, 0.22f, 0.38f, 1.0f));
    ApplyColor(RuntimeRightLegVisual, FLinearColor(0.16f, 0.22f, 0.38f, 1.0f));
    ApplyColor(RuntimeLeftBootVisual, FLinearColor(0.20f, 0.12f, 0.07f, 1.0f));
    ApplyColor(RuntimeRightBootVisual, FLinearColor(0.20f, 0.12f, 0.07f, 1.0f));
    ApplyColor(RuntimeHairVisual, FLinearColor(0.18f, 0.12f, 0.20f, 1.0f));
    ApplyColor(RuntimeHatVisual, FLinearColor(0.14f, 0.22f, 0.53f, 1.0f));
    ApplyColor(RuntimeBeltVisual, FLinearColor(0.80f, 0.56f, 0.15f, 1.0f));
    ApplyColor(RuntimeStaffVisual, FLinearColor(0.40f, 0.24f, 0.12f, 1.0f));
    ApplyColor(RuntimeOrbVisual, FLinearColor(0.32f, 0.82f, 0.98f, 1.0f));

    RuntimeBodyVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 12.0f));
    RuntimeBodyVisual->SetRelativeScale3D(FVector(0.42f, 0.39f, 0.82f));
    RuntimeHeadVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 66.0f));
    RuntimeHeadVisual->SetRelativeScale3D(FVector(0.34f, 0.32f, 0.37f));
    RuntimeMantleVisual->SetRelativeLocation(FVector(0.0f, 9.0f, -23.0f));
    RuntimeMantleVisual->SetRelativeScale3D(FVector(0.62f, 0.59f, 0.73f));

    RuntimeLeftArmVisual->SetRelativeLocation(FVector(-31.0f, 0.0f, 13.0f));
    RuntimeLeftArmVisual->SetRelativeRotation(FRotator(-23.0f, 0.0f, 0.0f));
    RuntimeLeftArmVisual->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.55f));
    RuntimeRightArmVisual->SetRelativeLocation(FVector(31.0f, 0.0f, 13.0f));
    RuntimeRightArmVisual->SetRelativeRotation(FRotator(23.0f, 0.0f, 0.0f));
    RuntimeRightArmVisual->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.55f));

    RuntimeLeftHandVisual->SetRelativeLocation(FVector(-49.0f, -3.0f, -12.0f));
    RuntimeLeftHandVisual->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));
    RuntimeRightHandVisual->SetRelativeLocation(FVector(49.0f, -3.0f, -12.0f));
    RuntimeRightHandVisual->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

    RuntimeLeftLegVisual->SetRelativeLocation(FVector(-15.0f, 0.0f, -61.0f));
    RuntimeLeftLegVisual->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.53f));
    RuntimeRightLegVisual->SetRelativeLocation(FVector(15.0f, 0.0f, -61.0f));
    RuntimeRightLegVisual->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.53f));
    RuntimeLeftBootVisual->SetRelativeLocation(FVector(-15.0f, -6.0f, -91.0f));
    RuntimeLeftBootVisual->SetRelativeScale3D(FVector(0.31f, 0.39f, 0.15f));
    RuntimeRightBootVisual->SetRelativeLocation(FVector(15.0f, -6.0f, -91.0f));
    RuntimeRightBootVisual->SetRelativeScale3D(FVector(0.31f, 0.39f, 0.15f));

    RuntimeHairVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
    RuntimeHairVisual->SetRelativeScale3D(FVector(0.36f, 0.34f, 0.17f));
    RuntimeHatVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 105.0f));
    RuntimeHatVisual->SetRelativeScale3D(FVector(0.38f, 0.38f, 0.46f));
    RuntimeBeltVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -22.0f));
    RuntimeBeltVisual->SetRelativeScale3D(FVector(0.44f, 0.42f, 0.10f));
    RuntimeStaffVisual->SetRelativeLocation(FVector(67.0f, -11.0f, 18.0f));
    RuntimeStaffVisual->SetRelativeScale3D(FVector(0.07f, 0.07f, 1.18f));
    RuntimeOrbVisual->SetRelativeLocation(FVector(67.0f, -11.0f, 82.0f));
    RuntimeOrbVisual->SetRelativeScale3D(FVector(0.22f, 0.22f, 0.22f));

    for (UStaticMeshComponent* Part : {
        RuntimeBodyVisual.Get(), RuntimeHeadVisual.Get(), RuntimeMantleVisual.Get(),
        RuntimeLeftArmVisual.Get(), RuntimeRightArmVisual.Get(), RuntimeLeftHandVisual.Get(),
        RuntimeRightHandVisual.Get(), RuntimeLeftLegVisual.Get(), RuntimeRightLegVisual.Get(),
        RuntimeLeftBootVisual.Get(), RuntimeRightBootVisual.Get(), RuntimeHairVisual.Get(),
        RuntimeHatVisual.Get(), RuntimeBeltVisual.Get(), RuntimeStaffVisual.Get(), RuntimeOrbVisual.Get()})
    {
        if (Part)
        {
            Part->SetCastShadow(true);
            Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
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
    AController* PreviousCharacterController = GetController();

    if (HasAuthority())
    {
        if (AAetherCharacterPlayerState* CharacterState = GetPlayerState<AAetherCharacterPlayerState>())
        {
            if (AAetherNetworkPlayerController* NetworkController = Cast<AAetherNetworkPlayerController>(PreviousCharacterController))
            {
                if (NetworkController->IsAccountAuthenticated())
                {
                    if (UAetherCharacterSubsystem* Characters = GetGameInstance()
                        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
                        : nullptr)
                    {
                        Characters->DeselectCharacter(
                            NetworkController->GetAuthenticatedAccountId(),
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

    InitializeRuntimeVisual();

    if (GetNetMode() == NM_DedicatedServer)
    {
        SetActorTickEnabled(false);
    }

    if (IsLocallyControlled())
    {
        InitializeFoundationInput();
    }
}

void AAetherCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateClickToMove(DeltaSeconds);
    UpdateSkeletalVisualAnimation();
}

void AAetherCharacter::InitializeRuntimeVisual()
{
    InitializeSkeletalVisual();

    const UAether2DCharacterVisualProfile* TwoDProfile = Visual2DComponent
        ? Visual2DComponent->GetProfile()
        : nullptr;
    const bool bUsePrimaryTwoDPresentation = TwoDProfile && TwoDProfile->bUseAsPrimaryPresentation;
    const bool bHasSkeletalPresentation = RuntimeSkeletalVisual && RuntimeSkeletalVisual->GetSkeletalMeshAsset() != nullptr;

    UStaticMeshComponent* ProxyParts[] = {
        RuntimeBodyVisual.Get(),
        RuntimeHeadVisual.Get(),
        RuntimeMantleVisual.Get(),
        RuntimeLeftArmVisual.Get(),
        RuntimeRightArmVisual.Get(),
        RuntimeLeftHandVisual.Get(),
        RuntimeRightHandVisual.Get(),
        RuntimeLeftLegVisual.Get(),
        RuntimeRightLegVisual.Get(),
        RuntimeLeftBootVisual.Get(),
        RuntimeRightBootVisual.Get(),
        RuntimeHairVisual.Get(),
        RuntimeHatVisual.Get(),
        RuntimeBeltVisual.Get(),
        RuntimeStaffVisual.Get(),
        RuntimeOrbVisual.Get()
    };

    for (UStaticMeshComponent* Part : ProxyParts)
    {
        if (Part)
        {
            Part->SetVisibility(!bUsePrimaryTwoDPresentation && !bHasSkeletalPresentation, true);
        }
    }

    if (RuntimeSkeletalVisual)
    {
        RuntimeSkeletalVisual->SetVisibility(bHasSkeletalPresentation && !bUsePrimaryTwoDPresentation, true);
    }

    UpdateSkeletalVisualAnimation();

    // Keep the old Paper2D component as a compatibility hook for authored profiles,
    // but never use the crude triangular runtime icon as the default character.
    if (Runtime2DArtVisual)
    {
        Runtime2DArtVisual->SetVisibility(false, true);
    }
}

void AAetherCharacter::InitializeSkeletalVisual()
{
    if (GetNetMode() == NM_DedicatedServer || !RuntimeSkeletalVisual)
    {
        return;
    }

    USkeletalMesh* LoadedSkeletalMesh = MageSkeletalMeshAsset.LoadSynchronous();
    if (!LoadedSkeletalMesh)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("AetherCharacter could not load skeletal mesh '%s'; keep the primitive fallback visible."),
            *MageSkeletalMeshAsset.ToSoftObjectPath().ToString());
        return;
    }

    RuntimeSkeletalVisual->SetSkeletalMesh(LoadedSkeletalMesh);
    RuntimeSkeletalVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RuntimeSkeletalVisual->SetCastShadow(true);
    RuntimeSkeletalVisual->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    RuntimeSkeletalVisual->SetRelativeLocation(FVector(
        0.0f,
        0.0f,
        -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
    // The imported Mage FBX is a quarter-turn off Unreal's +X movement-forward axis.
    RuntimeSkeletalVisual->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    RuntimeSkeletalVisual->SetRelativeScale3D(FVector::OneVector);

    // The owner's verified Editor assets retain the FBX suffix (for example,
    // A_Walk_Anim), while the canonical import script uses A_Walk. Prefer the
    // verified suffix variant, then fall back to the configured canonical path.
    const auto LoadMageAnimation = [](const TSoftObjectPtr<UAnimSequence>& ConfiguredAsset, const TCHAR* SuffixAssetPath)
    {
        if (UAnimSequence* Animation = LoadObject<UAnimSequence>(nullptr, SuffixAssetPath))
        {
            return Animation;
        }
        return ConfiguredAsset.LoadSynchronous();
    };

    RuntimeIdleAnimation = LoadMageAnimation(
        MageIdleAnimationAsset,
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Idle_Anim.A_Idle_Anim"));
    RuntimeWalkAnimation = LoadMageAnimation(
        MageWalkAnimationAsset,
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Walk_Anim.A_Walk_Anim"));
    RuntimeRunAnimation = LoadMageAnimation(
        MageRunAnimationAsset,
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Run_Anim.A_Run_Anim"));
    RuntimeJumpAnimation = LoadMageAnimation(
        MageJumpAnimationAsset,
        TEXT("/Game/Aether/Characters/Mage/Animations/A_Jump_Anim.A_Jump_Anim"));
    ActiveSkeletalAnimation = nullptr;

    if (!RuntimeIdleAnimation || !RuntimeWalkAnimation || !RuntimeRunAnimation || !RuntimeJumpAnimation)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("AetherCharacter loaded mage mesh '%s' with incomplete animation assets (idle=%s walk=%s run=%s jump=%s). Rerun Scripts/import_mage_assets.py in Unreal Editor."),
            *GetNameSafe(LoadedSkeletalMesh),
            *GetNameSafe(RuntimeIdleAnimation),
            *GetNameSafe(RuntimeWalkAnimation),
            *GetNameSafe(RuntimeRunAnimation),
            *GetNameSafe(RuntimeJumpAnimation));
    }
    else
    {
        UE_LOG(
            LogTemp,
            Log,
            TEXT("AetherCharacter loaded mage presentation: mesh=%s idle=%s walk=%s run=%s jump=%s"),
            *GetNameSafe(LoadedSkeletalMesh),
            *GetNameSafe(RuntimeIdleAnimation),
            *GetNameSafe(RuntimeWalkAnimation),
            *GetNameSafe(RuntimeRunAnimation),
            *GetNameSafe(RuntimeJumpAnimation));
    }

    UpdateSkeletalVisualAnimation();
}

void AAetherCharacter::UpdateSkeletalVisualAnimation()
{
    if (GetNetMode() == NM_DedicatedServer || !RuntimeSkeletalVisual || !RuntimeSkeletalVisual->GetSkeletalMeshAsset())
    {
        return;
    }

    const FVector Velocity = GetVelocity();
    const float PlanarSpeedSquared = Velocity.SizeSquared2D();
    const bool bIsMoving = PlanarSpeedSquared > FMath::Square(24.0f);
    const UCharacterMovementComponent* Movement = GetCharacterMovement();

    UAnimSequence* DesiredAnimation = nullptr;
    if (Movement && Movement->IsFalling() && RuntimeJumpAnimation)
    {
        DesiredAnimation = RuntimeJumpAnimation;
    }
    else if (bIsMoving)
    {
        const float WalkSpeed = MovementCameraProfile ? MovementCameraProfile->WalkSpeed : FoundationWalkSpeed;
        const bool bShouldRun = bSprinting || FMath::Sqrt(PlanarSpeedSquared) > WalkSpeed * 1.15f;
        DesiredAnimation = bShouldRun && RuntimeRunAnimation
            ? RuntimeRunAnimation.Get()
            : RuntimeWalkAnimation.Get();
    }
    else
    {
        DesiredAnimation = RuntimeIdleAnimation;
    }

    if (!DesiredAnimation && RuntimeIdleAnimation)
    {
        DesiredAnimation = RuntimeIdleAnimation;
    }
    if (!DesiredAnimation || DesiredAnimation == ActiveSkeletalAnimation)
    {
        return;
    }

    RuntimeSkeletalVisual->PlayAnimation(DesiredAnimation, true);
    ActiveSkeletalAnimation = DesiredAnimation;
    UE_LOG(LogTemp, Log, TEXT("AetherCharacter %s playing animation %s"), *GetName(), *DesiredAnimation->GetName());
}

void AAetherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    InitializeFoundationInput();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(ClickMoveAction, ETriggerEvent::Started, this, &AAetherCharacter::ClickMovePressed);
        EnhancedInput->BindAction(LookYawAction, ETriggerEvent::Triggered, this, &AAetherCharacter::LookYaw);
        EnhancedInput->BindAction(LookPitchAction, ETriggerEvent::Triggered, this, &AAetherCharacter::LookPitch);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AAetherCharacter::JumpPressed);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AAetherCharacter::SprintStarted);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAetherCharacter::SprintStopped);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AAetherCharacter::SprintStopped);
        EnhancedInput->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AAetherCharacter::CameraZoom);
        EnhancedInput->BindAction(CameraOrbitToggleAction, ETriggerEvent::Started, this, &AAetherCharacter::CameraOrbitToggle);
        EnhancedInput->BindAction(CameraOrbitDragAction, ETriggerEvent::Started, this, &AAetherCharacter::CameraOrbitDragStarted);
        EnhancedInput->BindAction(CameraOrbitDragAction, ETriggerEvent::Completed, this, &AAetherCharacter::CameraOrbitDragStopped);
        EnhancedInput->BindAction(CameraOrbitDragAction, ETriggerEvent::Canceled, this, &AAetherCharacter::CameraOrbitDragStopped);
        EnhancedInput->BindAction(CameraViewResetAction, ETriggerEvent::Started, this, &AAetherCharacter::CameraViewReset);
        EnhancedInput->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &AAetherCharacter::BasicAttackPressed);
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
    ClickMoveAction = NewObject<UInputAction>(this, TEXT("ClickMove"));
    LookYawAction = NewObject<UInputAction>(this, TEXT("LookYaw"));
    LookPitchAction = NewObject<UInputAction>(this, TEXT("LookPitch"));
    JumpAction = NewObject<UInputAction>(this, TEXT("Jump"));
    SprintAction = NewObject<UInputAction>(this, TEXT("Sprint"));
    CameraZoomAction = NewObject<UInputAction>(this, TEXT("CameraZoom"));
    CameraOrbitToggleAction = NewObject<UInputAction>(this, TEXT("CameraOrbitToggle"));
    CameraOrbitDragAction = NewObject<UInputAction>(this, TEXT("CameraOrbitDrag"));
    CameraViewResetAction = NewObject<UInputAction>(this, TEXT("CameraViewReset"));
    BasicAttackAction = NewObject<UInputAction>(this, TEXT("BasicAttack"));

    ClickMoveAction->ValueType = EInputActionValueType::Boolean;
    LookYawAction->ValueType = EInputActionValueType::Axis1D;
    LookPitchAction->ValueType = EInputActionValueType::Axis1D;
    JumpAction->ValueType = EInputActionValueType::Boolean;
    SprintAction->ValueType = EInputActionValueType::Boolean;
    CameraZoomAction->ValueType = EInputActionValueType::Axis1D;
    CameraOrbitToggleAction->ValueType = EInputActionValueType::Boolean;
    CameraOrbitDragAction->ValueType = EInputActionValueType::Boolean;
    CameraViewResetAction->ValueType = EInputActionValueType::Boolean;
    BasicAttackAction->ValueType = EInputActionValueType::Boolean;

    RuntimeInputContext->MapKey(ClickMoveAction, EKeys::LeftMouseButton);
    RuntimeInputContext->MapKey(LookYawAction, EKeys::MouseX);
    RuntimeInputContext->MapKey(LookPitchAction, EKeys::MouseY);
    RuntimeInputContext->MapKey(JumpAction, EKeys::SpaceBar);
    RuntimeInputContext->MapKey(SprintAction, EKeys::LeftShift);
    RuntimeInputContext->MapKey(CameraZoomAction, EKeys::MouseWheelAxis);
    RuntimeInputContext->MapKey(CameraOrbitToggleAction, EKeys::F5);
    RuntimeInputContext->MapKey(CameraOrbitDragAction, EKeys::MiddleMouseButton);
    RuntimeInputContext->MapKey(CameraViewResetAction, EKeys::F6);
    RuntimeInputContext->MapKey(BasicAttackAction, EKeys::RightMouseButton);

    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true;
    PC->bEnableMouseOverEvents = false;
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);

    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
        InputSubsystem->AddMappingContext(RuntimeInputContext, 0);
    }
}

void AAetherCharacter::ClickMovePressed(const FInputActionValue& Value)
{
    if (!Value.Get<bool>() || !IsLocallyControlled())
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC)
    {
        return;
    }

    FHitResult CursorHit;
    FVector Destination;
    if (PC->GetHitResultUnderCursorByChannel(
            UEngineTypes::ConvertToTraceType(ECC_Visibility), true, CursorHit))
    {
        Destination = CursorHit.ImpactPoint;
    }
    else
    {
        FVector RayOrigin;
        FVector RayDirection;
        if (!PC->DeprojectMousePositionToWorld(RayOrigin, RayDirection) || FMath::IsNearlyZero(RayDirection.Z))
        {
            return;
        }

        const float DistanceToGround = -RayOrigin.Z / RayDirection.Z;
        if (DistanceToGround < 0.0f)
        {
            return;
        }
        Destination = RayOrigin + RayDirection * DistanceToGround;
    }

    // The cursor can hit a house, tree, or the ground. Only the XY destination
    // matters; keep the character on its current floor and inside the arena.
    Destination.X = FMath::Clamp(Destination.X, -4050.0f, 4050.0f);
    Destination.Y = FMath::Clamp(Destination.Y, -3050.0f, 3050.0f);
    Destination.Z = GetActorLocation().Z;
    ClickMoveTarget = Destination;
    bHasClickMoveTarget = true;
}

void AAetherCharacter::UpdateClickToMove(float /*DeltaSeconds*/)
{
    if (!bHasClickMoveTarget || !IsLocallyControlled())
    {
        return;
    }

    FVector ToTarget = ClickMoveTarget - GetActorLocation();
    ToTarget.Z = 0.0f;
    if (ToTarget.SizeSquared2D() <= FMath::Square(55.0f))
    {
        bHasClickMoveTarget = false;
        UCharacterMovementComponent* Movement = GetCharacterMovement();
        if (Movement && !Movement->IsFalling())
        {
            Movement->StopMovementImmediately();
        }
        return;
    }

    AddMovementInput(ToTarget.GetSafeNormal2D());
}

void AAetherCharacter::LookYaw(const FInputActionValue& Value)
{
    if (IsometricCameraComponent && IsometricCameraComponent->HasUserOrbitView())
    {
        if (bCameraOrbitDragging && IsometricCameraComponent->IsFreeOrbitModeEnabled())
        {
            IsometricCameraComponent->AddOrbitInput(Value.Get<float>(), 0.0f);
        }
        return;
    }
    if (IsometricCameraComponent && !IsometricCameraComponent->AllowsFreeLook())
    {
        return;
    }
    AddControllerYawInput(Value.Get<float>() * CameraTurnRate);
}

void AAetherCharacter::LookPitch(const FInputActionValue& Value)
{
    if (IsometricCameraComponent && IsometricCameraComponent->HasUserOrbitView())
    {
        if (bCameraOrbitDragging && IsometricCameraComponent->IsFreeOrbitModeEnabled())
        {
            IsometricCameraComponent->AddOrbitInput(0.0f, Value.Get<float>());
        }
        return;
    }
    if (IsometricCameraComponent && !IsometricCameraComponent->AllowsFreeLook())
    {
        return;
    }
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
    if (IsometricCameraComponent)
    {
        IsometricCameraComponent->AddZoomInput(Value.Get<float>());
        return;
    }

    if (!CameraBoom) return;
    const float Step = MovementCameraProfile ? MovementCameraProfile->CameraZoomStep : 50.0f;
    const float MinDistance = MovementCameraProfile ? MovementCameraProfile->MinCameraDistance : 250.0f;
    const float MaxDistance = MovementCameraProfile ? MovementCameraProfile->MaxCameraDistance : 650.0f;
    CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - Value.Get<float>() * Step, MinDistance, MaxDistance);
}

void AAetherCharacter::CameraOrbitToggle(const FInputActionValue& Value)
{
    if (!Value.Get<bool>() || !IsLocallyControlled() || !IsometricCameraComponent)
    {
        return;
    }

    IsometricCameraComponent->ToggleFreeOrbitMode();
    if (!IsometricCameraComponent->IsFreeOrbitModeEnabled())
    {
        bCameraOrbitDragging = false;
    }
}

void AAetherCharacter::CameraOrbitDragStarted(const FInputActionValue& Value)
{
    bCameraOrbitDragging = Value.Get<bool>()
        && IsometricCameraComponent
        && IsometricCameraComponent->IsFreeOrbitModeEnabled();
}

void AAetherCharacter::CameraOrbitDragStopped(const FInputActionValue& /*Value*/)
{
    bCameraOrbitDragging = false;
}

void AAetherCharacter::CameraViewReset(const FInputActionValue& Value)
{
    if (Value.Get<bool>() && IsLocallyControlled() && IsometricCameraComponent)
    {
        IsometricCameraComponent->ResetCameraView();
    }
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

void AAetherCharacter::BasicAttackPressed(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        ExecuteBasicAttack();
    }
}

void AAetherCharacter::ExecuteBasicAttack()
{
    if (!IsLocallyControlled())
    {
        return;
    }

    if (VisualComponent)
    {
        VisualComponent->PlayBasicAttackAnimation();
    }

    if (Visual2DComponent)
    {
        Visual2DComponent->SetVisualState(EAether2DCharacterVisualState::Attack, false);
    }

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC)
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * 300.0f;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(AetherBasicAttack), true, this);
    Params.AddIgnoredActor(this);

    if (GetWorld() && GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, ECC_Pawn, Params))
    {
        if (AAetherCharacter* Target = Cast<AAetherCharacter>(Hit.GetActor()))
        {
            if (Target->GetCharacterId().IsValid())
            {
                ++LocalAttackSequence;
                if (AAetherNetworkPlayerController* NetworkController = Cast<AAetherNetworkPlayerController>(PC))
                {
                    NetworkController->BasicAttack(Target->GetCharacterId());
                }
            }
        }
    }
}

void AAetherCharacter::ServerRequestBasicAttack_Implementation(const FAetherCharacterId& TargetCharacterId)
{
    if (!HasAuthority() || !TargetCharacterId.IsValid())
    {
        return;
    }

    if (AAetherNetworkPlayerController* NetworkController = Cast<AAetherNetworkPlayerController>(GetController()))
    {
        NetworkController->BasicAttack(TargetCharacterId);
    }
}
