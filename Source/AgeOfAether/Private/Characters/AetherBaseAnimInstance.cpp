#include "Characters/AetherBaseAnimInstance.h"

#include "Characters/AetherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAetherBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
    if (!Character)
    {
        GroundSpeed = 0.0f;
        Direction = 0.0f;
        VerticalSpeed = 0.0f;
        bIsInAir = false;
        bIsSprinting = false;
        MovementState = EAetherAnimationMovementState::Idle;
        return;
    }

    RefreshMovementState(Character);
}

void UAetherBaseAnimInstance::RefreshMovementState(const ACharacter* Character)
{
    const FVector Velocity = Character->GetVelocity();
    GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
    VerticalSpeed = Velocity.Z;
    bIsInAir = Character->GetCharacterMovement() ? Character->GetCharacterMovement()->IsFalling() : false;

    const FVector Forward = Character->GetActorForwardVector();
    const FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.0f);
    Direction = HorizontalVelocity.IsNearlyZero()
        ? 0.0f
        : FMath::RadiansToDegrees(FMath::Atan2(
            FVector::CrossProduct(Forward, HorizontalVelocity.GetSafeNormal()).Z,
            FVector::DotProduct(Forward, HorizontalVelocity.GetSafeNormal())));

    const float WalkThreshold = AnimationProfile ? AnimationProfile->WalkSpeedThreshold : 5.0f;
    const float RunThreshold = AnimationProfile ? AnimationProfile->RunSpeedThreshold : 500.0f;

    if (bIsInAir)
    {
        MovementState = VerticalSpeed > 0.0f
            ? EAetherAnimationMovementState::Jump
            : EAetherAnimationMovementState::Fall;
    }
    else if (GroundSpeed < WalkThreshold)
    {
        MovementState = EAetherAnimationMovementState::Idle;
    }
    else if (GroundSpeed >= RunThreshold)
    {
        MovementState = EAetherAnimationMovementState::Run;
    }
    else
    {
        MovementState = EAetherAnimationMovementState::Walk;
    }

    if (const AAetherCharacter* AetherCharacter = Cast<AAetherCharacter>(Character))
    {
        const float CurrentMaxSpeed = AetherCharacter->GetCharacterMovement()
            ? AetherCharacter->GetCharacterMovement()->MaxWalkSpeed
            : RunThreshold;
        bIsSprinting = GroundSpeed >= RunThreshold && CurrentMaxSpeed >= RunThreshold;
    }
}