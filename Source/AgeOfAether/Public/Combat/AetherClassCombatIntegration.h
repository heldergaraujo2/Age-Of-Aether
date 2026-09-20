#pragma once
#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Combat/AetherCombatTypes.h"
#include "Data/AetherClassBalanceRegistry.h"
#include "Data/AetherClassRegistry.h"

class FAetherClassCombatIntegration
{
public:
    bool ResolveAuthoritativeContext(
        const FAetherCharacterRecord& Attacker,
        const FAetherCharacterRecord& Target,
        EAetherCombatMode Mode,
        const FAetherClassRegistry& ClassRegistry,
        const FAetherClassBalanceRegistry& BalanceRegistry,
        FAetherCombatBalanceContext& OutContext,
        FString& OutError) const;

    bool ValidateClientClassClaims(
        const FAetherCharacterRecord& AuthoritativeCharacter,
        const FString& ClientClassID,
        const FString& ClientEvolutionID,
        FString& OutError) const;
};
