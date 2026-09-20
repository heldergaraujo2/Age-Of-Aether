#include "Skills/AetherSkillSubsystem.h"
#include "Characters/AetherCharacterSubsystem.h"

namespace
{
    bool FindCharacter(UAetherSkillSubsystem* Self, const FAetherCharacterId& Id, FAetherCharacterRecord& Out)
    {
        UAetherCharacterSubsystem* Characters = Self && Self->GetGameInstance()
            ? Self->GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>() : nullptr;
        return Characters && Characters->FindCharacter(Id, Out);
    }
}

void UAetherSkillSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    FAetherSkillDefinition Training;
    Training.SkillID = TEXT("aether.skill.training_strike");
    Training.DisplayName = TEXT("Training Strike");
    Training.TargetMode = EAetherSkillTargetMode::SingleTarget;
    Training.CooldownSeconds = 1.0f;
    Training.Range = 300.0f;
    FAetherSkillEffect Damage;
    Damage.Type = EAetherSkillEffectType::Damage;
    Damage.Magnitude = 20.0f;
    Training.Effects.Add(Damage);
    RegisterSkill(Training);
}

void UAetherSkillSubsystem::Deinitialize()
{
    ResetRegistry();
    Super::Deinitialize();
}

bool UAetherSkillSubsystem::RegisterSkill(const FAetherSkillDefinition& Definition)
{
    return Registry.Register(Definition);
}

bool UAetherSkillSubsystem::RegisterSkills(const TArray<FAetherSkillDefinition>& Definitions)
{
    return Registry.RegisterBatch(Definitions);
}

const FAetherSkillDefinition* UAetherSkillSubsystem::FindSkill(const FString& SkillID) const
{
    return Registry.Find(SkillID);
}

bool UAetherSkillSubsystem::ValidateRegistry(TArray<FString>& OutErrors) const
{
    return Registry.Validate(OutErrors);
}

void UAetherSkillSubsystem::ResetRegistry()
{
    Registry.Reset();
    Cooldowns.Reset();
    ActiveStatuses.Reset();
}

FString UAetherSkillSubsystem::CooldownKey(const FAetherCharacterId& CharacterId, const FString& SkillID)
{
    return CharacterId.Value + TEXT("|") + SkillID.TrimStartAndEnd().ToLower();
}

bool UAetherSkillSubsystem::ApplyEffect(const FAetherSkillDefinition& Skill, const FAetherSkillEffect& Effect,
    const FAetherCharacterId& TargetId, FAetherSkillResult& OutResult)
{
    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>() : nullptr;
    if (!Characters) return false;

    FAetherCharacterRecord Target;
    if (!Characters->FindCharacter(TargetId, Target)) return false;

    if (Effect.Type == EAetherSkillEffectType::Damage)
    {
        if (Target.CombatState == EAetherCharacterCombatState::Dead) return false;
        const float Before = Target.CurrentHealth;
        Target.CurrentHealth = FMath::Clamp(Target.CurrentHealth - FMath::Max(0.0f, Effect.Magnitude), 0.0f, Target.DerivedStats.MaxHealth);
        if (Target.CurrentHealth <= 0.0f)
            Target.CombatState = EAetherCharacterCombatState::Dead;
        OutResult.PrimaryMagnitude += Before - Target.CurrentHealth;
        return Characters->ApplyCombatState(Target.AccountId, Target.CharacterId, Target);
    }

    if (Effect.Type == EAetherSkillEffectType::Heal)
    {
        if (Target.CombatState == EAetherCharacterCombatState::Dead) return false;
        const float Before = Target.CurrentHealth;
        Target.CurrentHealth = FMath::Clamp(Target.CurrentHealth + FMath::Max(0.0f, Effect.Magnitude), 0.0f, Target.DerivedStats.MaxHealth);
        OutResult.PrimaryMagnitude += Target.CurrentHealth - Before;
        return Characters->ApplyCombatState(Target.AccountId, Target.CharacterId, Target);
    }

    if (Effect.Type == EAetherSkillEffectType::Buff || Effect.Type == EAetherSkillEffectType::Debuff)
    {
        if (Effect.DurationSeconds <= 0.0f) return false;
        FAetherSkillRuntimeStatus Status;
        Status.EffectID = Effect.EffectID.TrimStartAndEnd().ToLower();
        Status.Type = Effect.Type;
        Status.Magnitude = Effect.Magnitude;
        Status.RemainingSeconds = Effect.DurationSeconds;
        ActiveStatuses.FindOrAdd(Target.CharacterId).Add(Status);
        OutResult.AppliedStatuses.Add(Status);
        return true;
    }

    return false;
}

bool UAetherSkillSubsystem::CastSkill(const FAetherAccountId& AccountId, const FAetherCharacterId& AttackerId,
    const FAetherCharacterId& TargetId, const FString& SkillID, uint32 RequestId,
    double ServerTimeSeconds, FAetherSkillResult& OutResult)
{
    OutResult = FAetherSkillResult();
    OutResult.RequestId = RequestId;
    OutResult.SkillID = SkillID.TrimStartAndEnd().ToLower();
    OutResult.TargetCharacterId = TargetId;

    if (!AccountId.IsValid() || !AttackerId.IsValid() || !TargetId.IsValid() || RequestId == 0 || OutResult.SkillID.IsEmpty())
    { OutResult.Result = EAetherSkillResultCode::InvalidRequest; return false; }

    const FAetherSkillDefinition* Skill = Registry.Find(OutResult.SkillID);
    if (!Skill) { OutResult.Result = EAetherSkillResultCode::SkillNotFound; return false; }
    if (!Skill->IsValid()) { OutResult.Result = EAetherSkillResultCode::InvalidDefinition; return false; }

    FAetherCharacterRecord Attacker;
    FAetherCharacterRecord Target;
    if (!FindCharacter(this, AttackerId, Attacker)) { OutResult.Result = EAetherSkillResultCode::NotOwned; return false; }
    if (!FindCharacter(this, TargetId, Target)) { OutResult.Result = EAetherSkillResultCode::InvalidTarget; return false; }
    if (Attacker.AccountId != AccountId) { OutResult.Result = EAetherSkillResultCode::NotOwned; return false; }
    if (!Skill->ClassID.IsEmpty() && !Attacker.ClassID.Equals(Skill->ClassID, ESearchCase::IgnoreCase))
    { OutResult.Result = EAetherSkillResultCode::NotOwned; return false; }
    if (Attacker.CombatState == EAetherCharacterCombatState::Dead) { OutResult.Result = EAetherSkillResultCode::AttackerDead; return false; }
    if (Target.CombatState == EAetherCharacterCombatState::Dead && Skill->TargetMode != EAetherSkillTargetMode::Self)
    { OutResult.Result = EAetherSkillResultCode::TargetDead; return false; }

    if (Skill->TargetMode == EAetherSkillTargetMode::Self && TargetId != AttackerId)
    { OutResult.Result = EAetherSkillResultCode::InvalidTarget; return false; }

    const float Distance = FVector::Dist(Attacker.WorldLocation, Target.WorldLocation);
    if (Distance > Skill->Range)
    { OutResult.Result = EAetherSkillResultCode::OutOfRange; return false; }

    const FString Key = CooldownKey(AttackerId, Skill->SkillID);
    if (const FCooldownState* Cooldown = Cooldowns.Find(Key))
    {
        if (ServerTimeSeconds < Cooldown->ReadyAt)
        { OutResult.Result = EAetherSkillResultCode::Cooldown; return false; }
    }

    for (const FAetherSkillEffect& Effect : Skill->Effects)
    {
        if (!ApplyEffect(*Skill, Effect, TargetId, OutResult))
        { OutResult.Result = EAetherSkillResultCode::InvalidDefinition; return false; }
    }

    FCooldownState& State = Cooldowns.FindOrAdd(Key);
    State.ReadyAt = ServerTimeSeconds + Skill->CooldownSeconds;
    OutResult.Result = EAetherSkillResultCode::Accepted;
    return true;
}
