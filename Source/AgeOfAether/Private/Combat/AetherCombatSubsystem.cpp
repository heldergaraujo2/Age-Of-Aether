#include "Combat/AetherCombatSubsystem.h"

#include "Characters/AetherCharacterSubsystem.h"

bool UAetherCombatSubsystem::BasicAttack(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& AttackerId,
    const FAetherCharacterId& TargetId,
    uint32 RequestId,
    double ServerTimeSeconds,
    FAetherCombatResult& OutResult)
{
    OutResult = FAetherCombatResult();
    OutResult.RequestId = RequestId;

    if (!AccountId.IsValid() || !AttackerId.IsValid() || !TargetId.IsValid() || RequestId == 0)
    {
        OutResult.Result = EAetherCombatResultCode::InvalidRequest;
        return false;
    }

    UAetherCharacterSubsystem* Characters = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>()
        : nullptr;

    if (!Characters)
    {
        OutResult.Result = EAetherCombatResultCode::InvalidState;
        return false;
    }

    FAetherCharacterRecord Attacker;
    FAetherCharacterRecord Target;

    if (!Characters->FindCharacter(AttackerId, Attacker))
    {
        OutResult.Result = EAetherCombatResultCode::AttackerNotFound;
        return false;
    }

    if (!Characters->FindCharacter(TargetId, Target))
    {
        OutResult.Result = EAetherCombatResultCode::TargetNotFound;
        return false;
    }

    if (Attacker.AccountId != AccountId)
    {
        OutResult.Result = EAetherCombatResultCode::NotOwned;
        return false;
    }

    FAetherCharacterRecord UpdatedTarget = Target;
    if (!CombatService.ResolveBasicAttack(
        Attacker,
        UpdatedTarget,
        RequestId,
        ServerTimeSeconds,
        OutResult))
    {
        return false;
    }

    if (OutResult.Result == EAetherCombatResultCode::Missed)
    {
        return true;
    }

    if (!Characters->ApplyCombatState(
        Target.AccountId,
        Target.CharacterId,
        UpdatedTarget))
    {
        OutResult.Result = EAetherCombatResultCode::DamageRejected;
        return false;
    }

    return true;
}

bool UAetherCombatSubsystem::IsAttackOnCooldown(
    const FAetherCharacterId& CharacterId,
    double ServerTimeSeconds,
    float& OutRemaining) const
{
    return CombatService.IsAttackOnCooldown(CharacterId, ServerTimeSeconds, OutRemaining);
}

const FAetherCombatConfig& UAetherCombatSubsystem::GetConfig() const
{
    return CombatService.GetConfig();
}

bool UAetherCombatSubsystem::Configure(const FAetherCombatConfig& NewConfig)
{
    return CombatService.SetConfig(NewConfig);
}


bool UAetherCombatSubsystem::ConfigureFromDataAsset(const UAetherCombatConfigDataAsset* ConfigAsset)
{
    return ConfigAsset && CombatService.SetConfig(ConfigAsset->Config);
}
