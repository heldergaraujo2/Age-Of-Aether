#include "UI/AetherFoundationHUD.h"

#include "Characters/AetherCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/Engine.h"

void AAetherFoundationHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas || !PlayerOwner)
    {
        return;
    }

    const APawn* Pawn = PlayerOwner->GetPawn();
    const AAetherCharacter* Character = Cast<AAetherCharacter>(Pawn);

    const FString Status = Character
        ? FString::Printf(TEXT("AGE OF AETHER | Visual Foundation | Character: %s"),
            *Character->GetName())
        : TEXT("AGE OF AETHER | Visual Foundation | Waiting for character");

    DrawText(Status, FLinearColor::White, 24.0f, 24.0f, GEngine ? GEngine->GetMediumFont() : nullptr, 1.0f, false);
    DrawText(TEXT("WASD Move  |  Mouse Look  |  Space Jump"),
        FLinearColor::White, 24.0f, 48.0f, GEngine ? GEngine->GetSmallFont() : nullptr, 1.0f, false);
    DrawText(PlayerOwner->HasAuthority()
        ? TEXT("Authority: SERVER")
        : TEXT("Authority: CLIENT / presentation only"),
        PlayerOwner->HasAuthority() ? FLinearColor::Yellow : FLinearColor::Green,
        24.0f, 70.0f, GEngine ? GEngine->GetSmallFont() : nullptr, 1.0f, false);
}
