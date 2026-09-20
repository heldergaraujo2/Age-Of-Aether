#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/AetherUITypes.h"
#include "AetherUIPresentationTypes.generated.h"

USTRUCT(BlueprintType)
struct FAetherUIScreenPresentation
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI") EAetherUIScreen Screen = EAetherUIScreen::None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI") TSoftClassPtr<class UUserWidget> WidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI") EAetherUIWindowMode DefaultMode = EAetherUIWindowMode::Fullscreen;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI") bool bKeepAlive = false;
    bool IsValid() const;
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherUIPresentationCatalog : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI") TArray<FAetherUIScreenPresentation> Screens;
    bool IsValid(TArray<FString>& OutErrors) const;
    const FAetherUIScreenPresentation* Find(EAetherUIScreen Screen) const;
};