#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Accounts/AetherAccountSessionTypes.h"
#include "Characters/AetherCharacterTypes.h"

#include "AetherCharacterPlayerState.generated.h"

UCLASS()
class AGEOFAETHER_API AAetherCharacterPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AAetherCharacterPlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void SetCharacterIdentity(const FAetherCharacterRecord& Character);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    FAetherAccountId GetAccountId() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    FAetherCharacterId GetCharacterId() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    FString GetCharacterName() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    EAetherCharacterClass GetCharacterClass() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    int32 GetCharacterLevel() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Progression")
    int64 GetCharacterExperience() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Progression")
    int32 GetUnspentStatPoints() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Progression")
    FAetherBaseStats GetBaseStats() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Progression")
    FAetherDerivedStats GetDerivedStats() const;

protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    FAetherAccountId AccountId;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    FAetherCharacterId CharacterId;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    FString CharacterName;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    EAetherCharacterClass CharacterClass = EAetherCharacterClass::Warrior;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    int32 CharacterLevel = 1;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Progression")
    int64 CharacterExperience = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Progression")
    int32 UnspentStatPoints = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Progression")
    FAetherBaseStats BaseStats;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Progression")
    FAetherDerivedStats DerivedStats;
};
