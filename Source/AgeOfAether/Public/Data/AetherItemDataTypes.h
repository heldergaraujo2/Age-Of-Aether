#pragma once
#include "CoreMinimal.h"
#include "AetherItemDataTypes.generated.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;

UENUM(BlueprintType)
enum class EAetherDataItemCategory : uint8
{
    Equipment, Consumable, Material, SkillItem, Quest, Currency, Cosmetic, Event, Container, Special
};

UENUM(BlueprintType)
enum class EAetherItemRarity : uint8
{
    Common, Uncommon, Rare, Epic, Legendary, Mythic
};

UENUM(BlueprintType)
enum class EAetherDataEquipmentSlot : uint8
{
    None, MainHand, OffHand, Shield, Head, Chest, Legs, Gloves, Feet, Ring1, Ring2, Necklace, Wings, Mount, Cosmetic
};

UENUM(BlueprintType)
enum class EAetherEnhancementFailureBehavior : uint8
{
    None, NoChange, Downgrade, Destroy
};

USTRUCT(BlueprintType)
struct FAetherItemRequirements
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 CharacterLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, int32> ClassMinimumLevels;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, int32> RequiredStats;
};

USTRUCT(BlueprintType)
struct FAetherItemBindingRules
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bBindOnPickup = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bBindOnEquip = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bTradable = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bSellable = true;
};

USTRUCT(BlueprintType)
struct FAetherItemEconomyRules
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString CurrencyID = TEXT("Currency.Gold");
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 BuyValue = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 SellValue = 0;
};

USTRUCT(BlueprintType)
struct FAetherEnhancementMaterial
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FAetherEnhancementLevel
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Level = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherEnhancementMaterial> Materials;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString CurrencyID = TEXT("Currency.Gold");
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 CurrencyAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double SuccessChance = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherEnhancementFailureBehavior FailureBehavior = EAetherEnhancementFailureBehavior::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ProtectionItemIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, double> StatScaling;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString VisualAssetID;
};

USTRUCT(BlueprintType)
struct FAetherItemVisualReferences
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString WorldMeshAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EquippedVisualAssetID;
};

USTRUCT(BlueprintType)
struct FAetherDataItemDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherDataItemCategory Category = EAetherDataItemCategory::Material;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherItemRarity Rarity = EAetherItemRarity::Common;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherDataEquipmentSlot EquipmentSlot = EAetherDataEquipmentSlot::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ItemLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredCharacterLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AllowedClasses;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxStack = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Weight = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 BaseDurability = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxDurability = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherItemRequirements Requirements;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherItemBindingRules BindingRules;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherItemEconomyRules Economy;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCanDrop = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCanShop = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCanCraft = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherItemVisualReferences Visuals;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, double> BaseStats;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, double> Options;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxEnhancementLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherEnhancementLevel> Enhancements;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherItemInstanceSnapshot
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString InstanceID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Quantity = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 ItemLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Durability = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 EnhancementLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bBound = false;
};

struct FAetherItemValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
