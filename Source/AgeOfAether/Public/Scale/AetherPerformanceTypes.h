#pragma once
#include "CoreMinimal.h"
#include "AetherPerformanceTypes.generated.h"

USTRUCT(BlueprintType)
struct FAetherPerformanceBudget
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadOnly) float TargetFrameMilliseconds=33.33f;
    UPROPERTY(EditAnywhere,BlueprintReadOnly) float MaxFrameMilliseconds=50.0f;
    UPROPERTY(EditAnywhere,BlueprintReadOnly) float MaxGameThreadMilliseconds=20.0f;
    UPROPERTY(EditAnywhere,BlueprintReadOnly) float MaxRenderThreadMilliseconds=20.0f;
    UPROPERTY(EditAnywhere,BlueprintReadOnly) float MaxStreamingMilliseconds=8.0f;
    UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 MaxActiveMapCount=8;
    UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 MaxVisibleCreatureCount=500;
    bool IsValid() const
    {
        return TargetFrameMilliseconds>0 && MaxFrameMilliseconds>=TargetFrameMilliseconds &&
            MaxGameThreadMilliseconds>0 && MaxRenderThreadMilliseconds>0 &&
            MaxStreamingMilliseconds>0 && MaxActiveMapCount>0 && MaxVisibleCreatureCount>0;
    }
};

USTRUCT(BlueprintType)
struct FAetherPerformanceSnapshot
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) double FrameMilliseconds=0;
    UPROPERTY(BlueprintReadOnly) double GameThreadMilliseconds=0;
    UPROPERTY(BlueprintReadOnly) double RenderThreadMilliseconds=0;
    UPROPERTY(BlueprintReadOnly) double StreamingMilliseconds=0;
    UPROPERTY(BlueprintReadOnly) int32 ActiveMapCount=0;
    UPROPERTY(BlueprintReadOnly) int32 VisibleCreatureCount=0;
    bool IsValid() const
    {
        return FrameMilliseconds>=0 && GameThreadMilliseconds>=0 && RenderThreadMilliseconds>=0 &&
            StreamingMilliseconds>=0 && ActiveMapCount>=0 && VisibleCreatureCount>=0;
    }
};
