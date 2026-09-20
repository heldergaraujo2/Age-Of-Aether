#include "UI/AetherUIController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
bool UAetherUIController::Initialize(APlayerController* InPlayerController, UAetherUIPresentationCatalog* InCatalog) { PlayerController=InPlayerController; Catalog=InCatalog; return IsInitialized(); }
UUserWidget* UAetherUIController::GetOrCreateWidget(EAetherUIScreen Screen)
{
    if (!IsInitialized()) return nullptr;
    if (TObjectPtr<UUserWidget>* Existing=KeepAliveWidgets.Find(Screen)) if (IsValid(*Existing)) return *Existing;
    const FAetherUIScreenPresentation* Entry=Catalog->Find(Screen); if (!Entry || Entry->WidgetClass.IsNull()) return nullptr;
    UClass* Class=Entry->WidgetClass.LoadSynchronous(); if (!Class) return nullptr;
    UUserWidget* Widget=CreateWidget<UUserWidget>(PlayerController.Get(),Class); if (!Widget) return nullptr;
    if (Entry->bKeepAlive) KeepAliveWidgets.Add(Screen,Widget);
    return Widget;
}
bool UAetherUIController::ShowScreen(EAetherUIScreen Screen)
{
    UUserWidget* Widget=GetOrCreateWidget(Screen); if (!Widget) return false;
    if (ActiveWidget && ActiveWidget!=Widget) ActiveWidget->RemoveFromParent();
    ActiveWidget=Widget; Widget->AddToViewport(10); return true;
}
bool UAetherUIController::CloseScreen() { if (!ActiveWidget) return false; ActiveWidget->RemoveFromParent(); ActiveWidget=nullptr; return true; }
bool UAetherUIController::ShowModal(const FString&) { return false; }
void UAetherUIController::CloseModal() { if (ModalWidget) ModalWidget->RemoveFromParent(); ModalWidget=nullptr; }
void UAetherUIController::Shutdown()
{
    CloseModal(); CloseScreen();
    for (auto& Pair:KeepAliveWidgets) if (IsValid(Pair.Value)) Pair.Value->RemoveFromParent();
    KeepAliveWidgets.Reset(); PlayerController.Reset(); Catalog.Reset();
}