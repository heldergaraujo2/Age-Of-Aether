#pragma once
#include "CoreMinimal.h"
#include "AetherClassBalanceTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherBalanceProfile : uint8
{
    Development,
    Testing,
    Production
};

USTRUCT(BlueprintType)
struct FAetherClassBalanceModifiers
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Damage = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CriticalDamage = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Defense = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Healing = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Shield = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double AreaDamage = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CrowdControl = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CrowdControlResistance = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double MovementSpeed = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double AttackSpeed = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CastSpeed = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Threat = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double LifeSteal = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Regeneration = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double BuffEffectiveness = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double DebuffEffectiveness = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double OutgoingDamage = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double IncomingDamage = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double ResourceCost = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double ResourceRecovery = 1.0;
    bool IsFinite() const;
    bool IsWithinSafetyBounds(FString& OutError) const;
    FAetherClassBalanceModifiers Multiply(const FAetherClassBalanceModifiers& Other) const;
};

USTRUCT(BlueprintType)
struct FAetherClassBalanceDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ClassID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherClassBalanceModifiers PvE;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherClassBalanceModifiers PvP;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherBalanceProfileDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ProfileID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Version = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherClassBalanceDefinition> Definitions;
};

USTRUCT(BlueprintType)
struct FAetherBalanceConfig
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ConfigVersion = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ActiveProfileID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString FallbackProfileID = TEXT("production");
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherBalanceProfileDefinition> Profiles;
};

struct FAetherBalanceValidationIssue
{
    FString ScopeID;
    FString Code;
    FString Message;
};
