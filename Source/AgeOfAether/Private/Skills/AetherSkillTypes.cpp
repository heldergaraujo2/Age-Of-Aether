#include "Skills/AetherSkillTypes.h"

bool FAetherSkillDefinition::IsValid() const
{
    if (SkillID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty())
        return false;
    if (!FMath::IsFinite(CooldownSeconds) || CooldownSeconds < 0.0f || CooldownSeconds > 3600.0f)
        return false;
    if (!FMath::IsFinite(Range) || Range < 0.0f || Range > 10000.0f)
        return false;
    if (!FMath::IsFinite(ResourceCost) || ResourceCost < 0.0f || ResourceCost > 1000000.0f)
        return false;

    for (const FAetherSkillEffect& Effect : Effects)
    {
        if (!FMath::IsFinite(Effect.Magnitude) || !FMath::IsFinite(Effect.DurationSeconds) ||
            !FMath::IsFinite(Effect.TickIntervalSeconds) ||
            Effect.DurationSeconds < 0.0f || Effect.DurationSeconds > 3600.0f ||
            Effect.TickIntervalSeconds < 0.0f || Effect.TickIntervalSeconds > 3600.0f)
            return false;
        if ((Effect.Type == EAetherSkillEffectType::Buff || Effect.Type == EAetherSkillEffectType::Debuff) &&
            Effect.EffectID.TrimStartAndEnd().IsEmpty())
            return false;
    }
    return !Effects.IsEmpty();
}
