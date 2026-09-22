#pragma once
#include "CoreMinimal.h"
#include "Data/AetherItemDataTypes.h"
#include "AetherLootTypes.generated.h"

USTRUCT(BlueprintType)
struct FAetherLootEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinQuantity=1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxQuantity=1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float DropChance=1.0f;
    bool IsValid() const { return !ItemID.IsEmpty() && MinQuantity>0 && MaxQuantity>=MinQuantity && FMath::IsFinite(DropChance) && DropChance>=0.f && DropChance<=1.f; }
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherLootTable : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString LootTableID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherLootEntry> Entries;
    UFUNCTION(BlueprintCallable) bool Validate(FString& OutError) const;
};