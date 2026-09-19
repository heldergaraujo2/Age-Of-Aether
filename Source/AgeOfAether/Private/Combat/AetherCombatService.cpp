#include "Combat/AetherCombatService.h"

FAetherCombatService::FAetherCombatService()
{
    Config = FAetherCombatConfig();
}

const FAetherCombatConfig& FAetherCombatService::GetConfig() const
{
    return Config;
}

bool FAetherCombatService::SetConfig(const FAetherCombatConfig& NewConfig)
{
    if (!IsConfigValid(NewConfig))
    {
        return false;
    }

    Config = NewConfig;
    return true;
}

bool FAetherCombatService::ResolveBasicAttack(
    const FAetherCharacterRecord& Attacker,
    FAetherCharacterRecord& Target,
    uint32 AttackSequence,
    double ServerTimeSeconds,
    FAetherCombatResult& OutResult)
{
    OutResult = FAetherCombatResult();

    if (!Attacker.CharacterId.IsValid() || !Target.CharacterId.IsValid() || AttackSequence == 0)
    {
        OutResult.Result = EAetherCombatResultCode::InvalidRequest;
        return false;
    }

    if (Attacker.CharacterId == Target.CharacterId)
    {
        OutResult.Result = EAetherCombatResultCode::InvalidRequest;
        return false;
    }

    if (Attacker.Status != EAetherCharacterStatus::Active)
    {
        OutResult.Result = EAetherCombatResultCode::InvalidState;
        return false;
    }

    if (Target.Status != EAetherCharacterStatus::Active)
    {
        OutResult.Result = EAetherCombatResultCode::InvalidState;
        return false;
    }

    if (Attacker.CombatState == EAetherCharacterCombatState::Dead)
    {
        OutResult.Result = EAetherCombatResultCode::AttackerDead;
        return false;
    }

    if (Target.CombatState == EAetherCharacterCombatState::Dead || Target.CurrentHealth <= 0.0f)
    {
        OutResult.Result = EAetherCombatResultCode::TargetDead;
        return false;
    }

    float CooldownRemaining = 0.0f;
    if (IsAttackOnCooldown(Attacker.CharacterId, ServerTimeSeconds, CooldownRemaining))
    {
        OutResult.Result = EAetherCombatResultCode::Cooldown;
        OutResult.CooldownRemaining = CooldownRemaining;
        return false;
    }

    const FVector Delta = Attacker.WorldLocation - Target.WorldLocation;
    const float Distance = Delta.Size();
    OutResult.Distance = Distance;

    if (!FMath::IsFinite(Distance) || Distance > Config.MaxAttackRange)
    {
        OutResult.Result = EAetherCombatResultCode::OutOfRange;
        return false;
    }

    const uint32 AccuracySeed = MakeRollSeed(Attacker.CharacterId, Target.CharacterId, AttackSequence, 0xA37E11u);
    const float AccuracyRoll = static_cast<float>(AccuracySeed % 10000u) / 10000.0f;

    NextAttackTimeByCharacter.Add(
        Attacker.CharacterId,
        ServerTimeSeconds + Config.AttackCooldownSeconds);

    if (AccuracyRoll >= Config.Accuracy)
    {
        OutResult.Result = EAetherCombatResultCode::Missed;
        OutResult.Outcome = EAetherCombatOutcome::Miss;
        return true;
    }

    const float MinAttack = FMath::Max(0.0f, Attacker.DerivedStats.AttackMin);
    const float MaxAttack = FMath::Max(MinAttack, Attacker.DerivedStats.AttackMax);
    const uint32 DamageSeed = MakeRollSeed(Attacker.CharacterId, Target.CharacterId, AttackSequence, 0xD4A6E1u);
    const float DamageRoll = static_cast<float>(DamageSeed % 10001u) / 10000.0f;
    const float BaseDamage = FMath::Lerp(MinAttack, MaxAttack, DamageRoll);

    const uint32 CriticalSeed = MakeRollSeed(Attacker.CharacterId, Target.CharacterId, AttackSequence, 0xC71A1u);
    const float CriticalRoll = static_cast<float>(CriticalSeed % 10000u) / 10000.0f;
    const bool bCritical = CriticalRoll < Config.CriticalChance;

    const float RawDamage = FMath::Max(
        Config.MinimumDamage,
        bCritical ? BaseDamage * Config.CriticalMultiplier : BaseDamage);

    const float Defense = FMath::Max(0.0f, Target.DerivedStats.Defense);
    const float MitigationDenominator = Config.DefenseMitigationScale + Defense;
    const float DamageAfterDefense = MitigationDenominator > 0.0f
        ? RawDamage * (Config.DefenseMitigationScale / MitigationDenominator)
        : RawDamage;
    const float DamageApplied = FMath::Clamp(
        DamageAfterDefense,
        0.0f,
        Target.CurrentHealth);

    OutResult.TargetHealthBefore = Target.CurrentHealth;
    OutResult.RawDamage = RawDamage;
    OutResult.DamageApplied = DamageApplied;
    OutResult.bCritical = bCritical;
    OutResult.Outcome = bCritical
        ? EAetherCombatOutcome::CriticalHit
        : EAetherCombatOutcome::Hit;
    OutResult.Result = EAetherCombatResultCode::Accepted;

    Target.CurrentHealth = FMath::Max(0.0f, Target.CurrentHealth - DamageApplied);
    Target.CurrentShield = FMath::Max(0.0f, Target.CurrentShield);
    if (Target.CurrentHealth <= 0.0f)
    {
        Target.CurrentHealth = 0.0f;
        Target.CombatState = EAetherCharacterCombatState::Dead;
        OutResult.Outcome = EAetherCombatOutcome::Defeated;
    }

    OutResult.TargetHealthAfter = Target.CurrentHealth;
    OutResult.TargetMaxHealth = Target.DerivedStats.MaxHealth;
    return true;
}

bool FAetherCombatService::IsAttackOnCooldown(
    const FAetherCharacterId& CharacterId,
    double ServerTimeSeconds,
    float& OutRemaining) const
{
    OutRemaining = 0.0f;

    const double* NextAttackTime = NextAttackTimeByCharacter.Find(CharacterId);
    if (!NextAttackTime)
    {
        return false;
    }

    const double Remaining = *NextAttackTime - ServerTimeSeconds;
    if (Remaining <= 0.0)
    {
        return false;
    }

    OutRemaining = static_cast<float>(Remaining);
    return true;
}

void FAetherCombatService::ClearCharacterCooldown(const FAetherCharacterId& CharacterId)
{
    NextAttackTimeByCharacter.Remove(CharacterId);
}

void FAetherCombatService::ClearAllCooldowns()
{
    NextAttackTimeByCharacter.Reset();
}

bool FAetherCombatService::IsConfigValid(const FAetherCombatConfig& NewConfig)
{
    return FMath::IsFinite(NewConfig.MaxAttackRange)
        && NewConfig.MaxAttackRange > 0.0f
        && FMath::IsFinite(NewConfig.AttackCooldownSeconds)
        && NewConfig.AttackCooldownSeconds >= 0.0f
        && FMath::IsFinite(NewConfig.Accuracy)
        && NewConfig.Accuracy >= 0.0f
        && NewConfig.Accuracy <= 1.0f
        && FMath::IsFinite(NewConfig.CriticalChance)
        && NewConfig.CriticalChance >= 0.0f
        && NewConfig.CriticalChance <= 1.0f
        && FMath::IsFinite(NewConfig.CriticalMultiplier)
        && NewConfig.CriticalMultiplier >= 1.0f
        && FMath::IsFinite(NewConfig.MinimumDamage)
        && NewConfig.MinimumDamage >= 0.0f
        && FMath::IsFinite(NewConfig.DefenseMitigationScale)
        && NewConfig.DefenseMitigationScale > 0.0f;
}

uint32 FAetherCombatService::MakeRollSeed(
    const FAetherCharacterId& AttackerId,
    const FAetherCharacterId& TargetId,
    uint32 AttackSequence,
    uint32 Salt)
{
    uint32 Seed = GetTypeHash(AttackerId);
    Seed = HashCombine(Seed, GetTypeHash(TargetId));
    Seed = HashCombine(Seed, GetTypeHash(AttackSequence));
    Seed = HashCombine(Seed, Salt);
    return Seed;
}
