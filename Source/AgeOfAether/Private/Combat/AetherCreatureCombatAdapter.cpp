#include "Combat/AetherCreatureCombatAdapter.h"

#include "Creatures/AetherCreatureActor.h"

FAetherCharacterRecord FAetherCreatureCombatAdapter::BuildCombatRecord(
    const FAetherCreatureDefinition& Definition,
    const AAetherCreatureActor& Creature)
{
    FAetherCharacterRecord Record;
    Record.CharacterId.Value = FString::Printf(TEXT("creature:%s"), *Definition.CreatureID.TrimStartAndEnd().ToLower());
    Record.Name = Definition.DisplayName;
    Record.ClassID = TEXT("creature");
    Record.EvolutionID = Definition.CreatureID.TrimStartAndEnd().ToLower();
    Record.Status = Creature.IsAlive()
        ? EAetherCharacterStatus::Active
        : EAetherCharacterStatus::Offline;
    Record.Level = Definition.Level;
    Record.DerivedStats.MaxHealth = Definition.MaxHealth;
    Record.DerivedStats.AttackMin = Definition.AttackMin;
    Record.DerivedStats.AttackMax = Definition.AttackMax;
    Record.DerivedStats.Defense = Definition.Defense;
    Record.DerivedStats.MoveSpeed = Definition.MoveSpeed;
    Record.CurrentHealth = Creature.GetRuntimeState().CurrentHealth;
    Record.CurrentShield = 0.0f;
    Record.CombatState = Creature.IsAlive()
        ? EAetherCharacterCombatState::Alive
        : EAetherCharacterCombatState::Dead;
    Record.WorldLocation = Creature.GetActorLocation();
    return Record;
}

bool FAetherCreatureCombatAdapter::ResolveBasicAttackAgainstCreature(
    FAetherCombatService& CombatService,
    const FAetherCharacterRecord& Attacker,
    const FAetherCreatureDefinition& TargetDefinition,
    AAetherCreatureActor& TargetCreature,
    uint32 AttackSequence,
    double ServerTimeSeconds,
    FAetherCombatResult& OutResult)
{
    OutResult = FAetherCombatResult();
    OutResult.RequestId = AttackSequence;

    if (!TargetCreature.IsAlive() || !TargetDefinition.IsValid())
    {
        OutResult.Result = TargetCreature.IsAlive()
            ? EAetherCombatResultCode::InvalidState
            : EAetherCombatResultCode::TargetDead;
        return false;
    }

    FAetherCharacterRecord Target = BuildCombatRecord(TargetDefinition, TargetCreature);
    const bool bResolved = CombatService.ResolveBasicAttack(
        Attacker,
        Target,
        AttackSequence,
        ServerTimeSeconds,
        OutResult);

    if (!bResolved || OutResult.Result == EAetherCombatResultCode::Missed)
    {
        return bResolved;
    }

    if (OutResult.Result != EAetherCombatResultCode::Accepted)
    {
        return false;
    }

    return TargetCreature.ApplyCombatDamage(OutResult.DamageApplied);
}

bool FAetherCreatureCombatAdapter::ResolveCreatureAttackAgainstCharacter(
    FAetherCombatService& CombatService,
    const FAetherCreatureDefinition& AttackerDefinition,
    const AAetherCreatureActor& AttackerCreature,
    FAetherCharacterRecord& TargetCharacter,
    uint32 AttackSequence,
    double ServerTimeSeconds,
    FAetherCombatResult& OutResult)
{
    OutResult = FAetherCombatResult();
    OutResult.RequestId = AttackSequence;

    if (!AttackerCreature.IsAlive() || !AttackerDefinition.IsValid())
    {
        OutResult.Result = AttackerCreature.IsAlive()
            ? EAetherCombatResultCode::InvalidState
            : EAetherCombatResultCode::AttackerDead;
        return false;
    }

    const FAetherCharacterRecord Attacker = BuildCombatRecord(AttackerDefinition, AttackerCreature);
    return CombatService.ResolveBasicAttack(
        Attacker,
        TargetCharacter,
        AttackSequence,
        ServerTimeSeconds,
        OutResult);
}
