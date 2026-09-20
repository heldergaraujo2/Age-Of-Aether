#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "AetherGameInstance.generated.h"

UCLASS(Config=Game)
class AGEOFAETHER_API UAetherGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UAetherGameInstance();

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Foundation")
    static UAetherGameInstance* GetAether(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Foundation")
    FName GetDevelopmentMapId() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Foundation")
    int32 GetVisualFoundationVersion() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Age of Aether|Foundation")
    FName DevelopmentMapId = TEXT("aether_development");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Age of Aether|Foundation")
    int32 VisualFoundationVersion = 1;
};
