#pragma once
#include "CoreMinimal.h"
#include "World/AetherWorldTypes.h"
#include "AetherWorldMapTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherWorldMapLoadMode : uint8 { Persistent, Streamed, Instance };

USTRUCT(BlueprintType)
struct FAetherWorldMapDefinition
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString MapID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString DisplayName;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString MapAssetPath;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) EAetherWorldMapLoadMode LoadMode=EAetherWorldMapLoadMode::Streamed;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FAetherWorldZoneId ZoneID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bEnabled=true;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bServerRequired=true;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bClientPresentation=true;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 MinimumLevel=1;
 bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherWorldStreamingLink
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString LinkID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString SourceMapID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString TargetMapID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bPreload=false;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bEnabled=true;
 bool IsValid() const;
};