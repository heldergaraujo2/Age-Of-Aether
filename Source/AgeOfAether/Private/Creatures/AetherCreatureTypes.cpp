#include "Creatures/AetherCreatureTypes.h"

bool FAetherCreatureDefinition::IsValid() const
{
    if (CreatureID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty())
        return false;
    if (!FMath::IsFinite(MaxHealth) || MaxHealth <= 0.0f || MaxHealth > 1000000000.0f)
        return false;
    if (!FMath::IsFinite(Defense) || Defense < 0.0f || Defense > 1000000000.0f)
        return false;
    if (!FMath::IsFinite(AttackMin) || !FMath::IsFinite(AttackMax) ||
        AttackMin < 0.0f || AttackMax < AttackMin || AttackMax > 1000000000.0f)
        return false;
    if (!FMath::IsFinite(MoveSpeed) || MoveSpeed < 0.0f || MoveSpeed > 10000.0f)
        return false;
    if (Level < 1 || Level > 100000)
        return false;
    if (Role == EAetherCreatureRole::Boss && MaxHealth < 100.0f)
        return false;
    return true;
}

bool FAetherCreatureSpawnDefinition::IsValid() const
{
    return !SpawnID.TrimStartAndEnd().IsEmpty() &&
        !CreatureID.TrimStartAndEnd().IsEmpty() &&
        ZoneID.IsValid() &&
        MaxAlive > 0 && MaxAlive <= 10000 && FMath::IsFinite(RespawnSeconds) && RespawnSeconds >= 0.0f && RespawnSeconds <= 86400.0f;
}
