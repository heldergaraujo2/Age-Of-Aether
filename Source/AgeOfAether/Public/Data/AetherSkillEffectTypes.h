#pragma once
#include "CoreMinimal.h"
#include "AetherSkillEffectTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherSkillTarget : uint8 { Self, SingleAlly, SingleEnemy, AreaAlly, AreaEnemy, AreaAll, Party, Ground };
UENUM(BlueprintType)
enum class EAetherSkillDelivery : uint8 { Instant, Projectile, Beam, Area, Melee, Ground };
UENUM(BlueprintType)
enum class EAetherEffectType : uint8 { Damage, Heal, ManaRestore, Stun, Root, Silence, Slow, Haste, Shield, DamageOverTime, HealOverTime, StatModifier, CrowdControlImmunity, Taunt, Dispel, Custom };
UENUM(BlueprintType)
enum class EAetherEffectStackRule : uint8 { Replace, RefreshDuration, AddStacks, Independent };
UENUM(BlueprintType)
enum class EAetherEffectDispelRule : uint8 { None, Dispellable, Undispellable };

USTRUCT(BlueprintType)
struct FAetherSkillCost { GENERATED_BODY() UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Mana=0; UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Health=0; UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Resource=0; UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ResourceID; };

USTRUCT(BlueprintType)
struct FAetherDataSkillDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredLevel=1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherSkillTarget Target=EAetherSkillTarget::SingleEnemy;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherSkillDelivery Delivery=EAetherSkillDelivery::Instant;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CastTimeSeconds=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CooldownSeconds=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Range=1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Radius=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherSkillCost Cost;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> BlockedTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString AnimationAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString VisualAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCanCrit=false;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherEffectDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherEffectType Type=EAetherEffectType::Custom;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double DurationSeconds=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double TickIntervalSeconds=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Magnitude=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double SecondaryMagnitude=0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxStacks=1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherEffectStackRule StackRule=EAetherEffectStackRule::Replace;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherEffectDispelRule DispelRule=EAetherEffectDispelRule::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ConflictingEffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString VisualAssetID;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherStatusDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> GrantedTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RemovedTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bVisibleToClient=true;
    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherSkillEffectValidationIssue { FString DefinitionID; FString Code; FString Message; };
