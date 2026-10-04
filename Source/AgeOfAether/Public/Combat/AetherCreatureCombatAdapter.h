#pragma once

#include "CoreMinimal.h"
#include "Combat/AetherCombatService.h"
#include "Creatures/AetherCreatureTypes.h"

class AAetherCreatureActor;

class AGEOFAETHER_API FAetherCreatureCombatAdapter
{
public:
    static FAetherCharacterRecord BuildCombatRecord(
        const FAetherCreatureDefinition& Definition,
        const AAetherCreatureActor& Creature);

    static bool ResolveBasicAttackAgainstCreature(
        FAetherCombatService& CombatService,
        const FAetherCharacterRecord& Attacker,
        const FAetherCreatureDefinition& TargetDefinition,
        AAetherCreatureActor& TargetCreature,
        uint32 AttackSequence,
        double ServerTimeSeconds,
        FAetherCombatResult& OutResult);

    static bool ResolveCreatureAttackAgainstCharacter(
        FAetherCombatService& CombatService,
        const FAetherCreatureDefinition& AttackerDefinition,
        const AAetherCreatureActor& AttackerCreature,
        FAetherCharacterRecord& TargetCharacter,
        uint32 AttackSequence,
        double ServerTimeSeconds,
        FAetherCombatResult& OutResult);
};
