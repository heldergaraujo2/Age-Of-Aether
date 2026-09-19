#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Characters/AetherCharacterTypes.h"

#include "AetherNetworkGameMode.generated.h"

UCLASS()
class AGEOFAETHER_API AAetherNetworkGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAetherNetworkGameMode();
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    void SpawnSelectedCharacter(APlayerController* PlayerController, const FAetherCharacterRecord& Character);
};
