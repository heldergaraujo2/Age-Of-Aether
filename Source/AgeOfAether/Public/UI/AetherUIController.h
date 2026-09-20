#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UI/AetherUITypes.h"
#include "UI/AetherUIPresentationTypes.h"
#include "AetherUIController.generated.h"
class UUserWidget; class APlayerController;
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherUIController : public UObject
{
    GENERATED_BODY()
public:
    bool Initialize(APlayerController* InPlayerController, UAetherUIPresentationCatalog* InCatalog);
    bool ShowScreen(EAetherUIScreen Screen);
    bool CloseScreen();
    bool ShowModal(const FString& ModalID);
    void CloseModal();
    void Shutdown();
    UFUNCTION(BlueprintPure, Category="Age of Aether|UI") bool IsInitialized() const { return PlayerController.IsValid() && Catalog.IsValid(); }
    UFUNCTION(BlueprintPure, Category="Age of Aether|UI") UUserWidget* GetActiveWidget() const { return ActiveWidget; }
private:
    TWeakObjectPtr<APlayerController> PlayerController;
    TWeakObjectPtr<UAetherUIPresentationCatalog> Catalog;
    TMap<EAetherUIScreen, TObjectPtr<UUserWidget>> KeepAliveWidgets;
    TObjectPtr<UUserWidget> ActiveWidget;
    TObjectPtr<UUserWidget> ModalWidget;
    UUserWidget* GetOrCreateWidget(EAetherUIScreen Screen);
};