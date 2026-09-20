#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Combat/AetherCombatTypes.h"

class FAetherCombatService
{
public:
    FAetherCombatService();

    const FAetherCombatConfig& GetConfig() const;
    bool SetConfig(const FAetherCombatConfig& NewConfig);

    bool ResolveBasicAttack(
        const FAetherCharacterRecord& Attacker,
        FAetherCharacterRecord& Target,
        uint32 AttackSequence,
        double ServerTimeSeconds,
        FAetherCombatResult& OutResult);

    bool ResolveBasicAttackAuthoritative(
        const FAetherCharacterRecord& Attacker,
        FAetherCharacterRecord& Target,
        uint32 AttackSequence,
        double ServerTimeSeconds,
        const FAetherCombatBalanceContext& BalanceContext,
        FAetherCombatResult& OutResult);

    bool IsAttackOnCooldown(
        const FAetherCharacterId& CharacterId,
        double ServerTimeSeconds,
        float& OutRemaining) const;

    void ClearCharacterCooldown(const FAetherCharacterId& CharacterId);
    void ClearAllCooldowns();

private:
    static bool IsConfigValid(const FAetherCombatConfig& Config);
    static uint32 MakeRollSeed(
        const FAetherCharacterId& AttackerId,
        const FAetherCharacterId& TargetId,
        uint32 AttackSequence,
        uint32 Salt);

    FAetherCombatConfig Config;
    TMap<FAetherCharacterId, double> NextAttackTimeByCharacter;
};
