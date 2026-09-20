#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Skills/AetherSkillTypes.h"
#include "Skills/AetherSkillRegistry.h"
#include "AetherSkillSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherSkillSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    bool RegisterSkill(const FAetherSkillDefinition& Definition);
    bool RegisterSkills(const TArray<FAetherSkillDefinition>& Definitions);
    bool CastSkill(const FAetherAccountId& AccountId, const FAetherCharacterId& AttackerId,
        const FAetherCharacterId& TargetId, const FString& SkillID, uint32 RequestId,
        double ServerTimeSeconds, FAetherSkillResult& OutResult);

    const FAetherSkillDefinition* FindSkill(const FString& SkillID) const;
    bool ValidateRegistry(TArray<FString>& OutErrors) const;
    void ResetRegistry();

private:
    struct FCooldownState
    {
        double ReadyAt = 0.0;
    };

    FAetherSkillRegistry Registry;
    TMap<FString, FCooldownState> Cooldowns;
    TMap<FAetherCharacterId, TArray<FAetherSkillRuntimeStatus>> ActiveStatuses;

    static FString CooldownKey(const FAetherCharacterId& CharacterId, const FString& SkillID);
    bool ApplyEffect(const FAetherSkillDefinition& Skill, const FAetherSkillEffect& Effect,
        const FAetherCharacterId& TargetId, FAetherSkillResult& OutResult);
};
