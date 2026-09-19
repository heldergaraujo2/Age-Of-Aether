#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Characters/AetherCharacterTypes.h"
#include "Combat/AetherCombatService.h"

#include "AetherCombatSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherCombatSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool BasicAttack(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& AttackerId,
        const FAetherCharacterId& TargetId,
        uint32 RequestId,
        double ServerTimeSeconds,
        FAetherCombatResult& OutResult);

    bool IsAttackOnCooldown(
        const FAetherCharacterId& CharacterId,
        double ServerTimeSeconds,
        float& OutRemaining) const;

    const FAetherCombatConfig& GetConfig() const;
    bool Configure(const FAetherCombatConfig& NewConfig);

private:
    FAetherCombatService CombatService;
};
