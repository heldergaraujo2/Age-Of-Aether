#pragma once

#include "CoreMinimal.h"

#include "AetherRuntimeTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherRuntimeState : uint8
{
    Uninitialized,
    Initializing,
    Running,
    ShuttingDown,
    Stopped,
    Failed
};

UENUM(BlueprintType)
enum class EAetherHealthState : uint8
{
    Unknown,
    Healthy,
    Degraded,
    Unhealthy
};

UENUM(BlueprintType)
enum class EAetherRuntimeError : uint8
{
    None,
    AlreadyInitialized,
    NotInitialized,
    InvalidTransition,
    ServiceUnavailable,
    InvalidConfiguration,
    SchedulerFailure
};

USTRUCT(BlueprintType)
struct FAetherRuntimeResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bSucceeded = false;

    UPROPERTY(BlueprintReadOnly)
    EAetherRuntimeError Error = EAetherRuntimeError::None;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    static FAetherRuntimeResult Success(const FString& InMessage = FString())
    {
        FAetherRuntimeResult Result;
        Result.bSucceeded = true;
        Result.Message = InMessage;
        return Result;
    }

    static FAetherRuntimeResult Failure(EAetherRuntimeError InError, const FString& InMessage)
    {
        FAetherRuntimeResult Result;
        Result.bSucceeded = false;
        Result.Error = InError;
        Result.Message = InMessage;
        return Result;
    }
};
