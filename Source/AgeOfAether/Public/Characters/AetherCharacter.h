#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/AetherCharacterTypes.h"

#include "AetherCharacter.generated.h"

UCLASS()
class AGEOFAETHER_API AAetherCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAetherCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    FAetherCharacterId GetCharacterId() const;

    void InitializeCharacterIdentity(const FAetherCharacterId& InCharacterId);

protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    FAetherCharacterId CharacterId;
};
