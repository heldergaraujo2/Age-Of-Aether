#pragma once
#include "CoreMinimal.h"
#include "AetherPresentationTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherPresentationActorState : uint8
{
    Hidden,
    Spawning,
    Idle,
    Moving,
    Attacking,
    Casting,
    HitReact,
    Dead,
    Emote,
    Mount
};

UENUM(BlueprintType)
enum class EAetherPerformanceMetric : uint8
{
    Frame,
    GameThread,
    RenderThread,
    GPU,
    DrawCalls,
    MemoryMB,
    TextureMemoryMB,
    AnimationMS,
    VFXMS,
    ActorCount,
    ReplicationKbps
};

UENUM(BlueprintType)
enum class EAetherClientAcceptanceStep : uint8
{
    Launch,
    Login,
    Authentication,
    CharacterList,
    CharacterCreation,
    CharacterSelection,
    WorldLoading,
    CharacterSpawn,
    Movement,
    Camera,
    Interaction,
    Targeting,
    Combat,
    Skills,
    Inventory,
    Equipment,
    Loot,
    Crafting,
    NPC,
    Shop,
    Quests,
    Party,
    Guild,
    Chat,
    Map,
    Persistence,
    Reconnect,
    DisconnectRecovery,
    SecurityRejection,
    DedicatedServerConnection
};

USTRUCT(BlueprintType)
struct FAetherAssetRuntimeBinding
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString AssetID;
    UPROPERTY(BlueprintReadOnly) FString RuntimePath;
    UPROPERTY(BlueprintReadOnly) FString PlaceholderPath;
    UPROPERTY(BlueprintReadOnly) bool bResolved = false;

    bool IsUsable() const;
};

USTRUCT(BlueprintType)
struct FAetherPresentationActorSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString EntityID;
    UPROPERTY(BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(BlueprintReadOnly) FString MeshAssetID;
    UPROPERTY(BlueprintReadOnly) FString AnimationAssetID;
    UPROPERTY(BlueprintReadOnly) FString VFXAssetID;
    UPROPERTY(BlueprintReadOnly) FString SFXAssetID;
    UPROPERTY(BlueprintReadOnly) EAetherPresentationActorState State = EAetherPresentationActorState::Hidden;
    UPROPERTY(BlueprintReadOnly) float MoveSpeed = 0.0f;
    UPROPERTY(BlueprintReadOnly) float NormalizedHealth = 1.0f;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherPerformanceSample
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) EAetherPerformanceMetric Metric = EAetherPerformanceMetric::Frame;
    UPROPERTY(BlueprintReadOnly) double Value = 0.0;
    UPROPERTY(BlueprintReadOnly) double Budget = 0.0;
    UPROPERTY(BlueprintReadOnly) double TimestampSeconds = 0.0;

    bool IsWithinBudget() const;
};

USTRUCT(BlueprintType)
struct FAetherClientAcceptanceResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) EAetherClientAcceptanceStep Step = EAetherClientAcceptanceStep::Launch;
    UPROPERTY(BlueprintReadOnly) bool bPassed = false;
    UPROPERTY(BlueprintReadOnly) FString Diagnostic;

    void Reset();
};
