#include "Characters/AetherCharacter.h"

#include "Characters/AetherCharacterPlayerState.h"
#include "Characters/AetherCharacterSubsystem.h"
#include "Networking/AetherNetworkPlayerController.h"

#include "Net/UnrealNetwork.h"

AAetherCharacter::AAetherCharacter()
{
    bReplicates = true;
    SetReplicateMovement(true);
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
