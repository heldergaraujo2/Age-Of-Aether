#pragma once
#include "CoreMinimal.h"
#include "AetherUITypes.generated.h"

UENUM(BlueprintType)
enum class EAetherUIScreen : uint8
{
    None,
    Login,
    CharacterSelect,
    Loading,
    HUD,
    Character,
    Inventory,
    Equipment,
    Skills,
    Craft,
    NPC,
    Quests,
    Social,
    Map,
    Settings
};

UENUM(BlueprintType)
enum class EAetherUINotificationType : uint8
{
    Info,
    Success,
    Warning,
    Error,
    System
};

UENUM(BlueprintType)
enum class EAetherUIWindowMode : uint8
{
    Fullscreen,
    Overlay,
    Modal
};


USTRUCT(BlueprintType)
struct FAetherUIClassEntry
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FString ClassID;
    UPROPERTY(BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) FString PathTitle;
    UPROPERTY(BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(BlueprintReadOnly) bool bSelected = false;
    UPROPERTY(BlueprintReadOnly) bool bUnlocked = false;
};

USTRUCT(BlueprintType)
struct FAetherUIItemEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString InstanceID;
    UPROPERTY(BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(BlueprintReadOnly) int32 Quantity = 1;
    UPROPERTY(BlueprintReadOnly) int32 SlotIndex = -1;
    UPROPERTY(BlueprintReadOnly) int32 ItemLevel = 0;
    UPROPERTY(BlueprintReadOnly) int32 EnhancementLevel = 0;
    UPROPERTY(BlueprintReadOnly) float Durability = 0.0f;
    UPROPERTY(BlueprintReadOnly) bool bBound = false;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherUIEquipmentEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString SlotID;
    UPROPERTY(BlueprintReadOnly) FAetherUIItemEntry Item;
    UPROPERTY(BlueprintReadOnly) bool bOccupied = false;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherUISkillEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString SkillID;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(BlueprintReadOnly) int32 SkillLevel = 0;
    UPROPERTY(BlueprintReadOnly) double CooldownRemaining = 0.0;
    UPROPERTY(BlueprintReadOnly) bool bLearned = false;
    UPROPERTY(BlueprintReadOnly) bool bAssignedToHotbar = false;
    UPROPERTY(BlueprintReadOnly) int32 HotbarSlot = -1;
};

USTRUCT(BlueprintType)
struct FAetherUIQuestObjective
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString ObjectiveID;
    UPROPERTY(BlueprintReadOnly) FString Description;
    UPROPERTY(BlueprintReadOnly) int32 Current = 0;
    UPROPERTY(BlueprintReadOnly) int32 Required = 0;
    UPROPERTY(BlueprintReadOnly) bool bOptional = false;
    UPROPERTY(BlueprintReadOnly) bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FAetherUIQuestEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString QuestID;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) FString Description;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUIQuestObjective> Objectives;
    UPROPERTY(BlueprintReadOnly) bool bTracked = false;
    UPROPERTY(BlueprintReadOnly) bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FAetherUISocialEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString CharacterID;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) FString GuildName;
    UPROPERTY(BlueprintReadOnly) bool bOnline = false;
    UPROPERTY(BlueprintReadOnly) bool bBlocked = false;
    UPROPERTY(BlueprintReadOnly) bool bPartyMember = false;
};

USTRUCT(BlueprintType)
struct FAetherUINotification
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) uint32 NotificationID = 0;
    UPROPERTY(BlueprintReadOnly) EAetherUINotificationType Type = EAetherUINotificationType::Info;
    UPROPERTY(BlueprintReadOnly) FString Title;
    UPROPERTY(BlueprintReadOnly) FString Message;
    UPROPERTY(BlueprintReadOnly) double CreatedAtSeconds = 0.0;
    UPROPERTY(BlueprintReadOnly) double DurationSeconds = 5.0;

    bool IsExpired(double NowSeconds) const;
};

USTRUCT(BlueprintType)
struct FAetherUISettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) float UIScale = 1.0f;
    UPROPERTY(BlueprintReadOnly) bool bShowDamageNumbers = true;
    UPROPERTY(BlueprintReadOnly) bool bShowQuestTracker = true;
    UPROPERTY(BlueprintReadOnly) bool bShowMinimap = true;
    UPROPERTY(BlueprintReadOnly) bool bColorblindAssist = false;
    UPROPERTY(BlueprintReadOnly) FString Language = TEXT("en");

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherUIRuntimeSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) EAetherUIScreen CurrentScreen = EAetherUIScreen::None;
    UPROPERTY(BlueprintReadOnly) EAetherUIScreen PreviousScreen = EAetherUIScreen::None;
    UPROPERTY(BlueprintReadOnly) EAetherUIWindowMode WindowMode = EAetherUIWindowMode::Fullscreen;
    UPROPERTY(BlueprintReadOnly) bool bModalOpen = false;
    UPROPERTY(BlueprintReadOnly) FString ModalID;
    UPROPERTY(BlueprintReadOnly) int32 InventoryCapacity = 0;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUIClassEntry> ClassSelection;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUIItemEntry> Inventory;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUIEquipmentEntry> Equipment;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUISkillEntry> Skills;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUIQuestEntry> Quests;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUISocialEntry> Social;
    UPROPERTY(BlueprintReadOnly) TArray<FAetherUINotification> Notifications;
    UPROPERTY(BlueprintReadOnly) FAetherUISettings Settings;

    void Reset();
};
