#include "UI/AetherUISubsystem.h"

void UAetherUISubsystem::Deinitialize()
{
    ResetUI();
    Super::Deinitialize();
}

bool UAetherUISubsystem::IsNavigable(EAetherUIScreen Screen) const
{
    return Screen != EAetherUIScreen::None && Screen != EAetherUIScreen::Loading;
}

void UAetherUISubsystem::ResetUI()
{
    Snapshot.Reset();
    History.Reset();
    NextNotificationID = 1;
    OnStateChanged.Broadcast();
}

bool UAetherUISubsystem::OpenScreen(EAetherUIScreen Screen, EAetherUIWindowMode Mode)
{
    if (!IsNavigable(Screen)) return false;
    if (Snapshot.bModalOpen) return false;
    if (Snapshot.CurrentScreen == Screen)
    {
        Snapshot.WindowMode = Mode;
        OnStateChanged.Broadcast();
        return true;
    }

    if (Snapshot.CurrentScreen != EAetherUIScreen::None)
        History.Add(Snapshot.CurrentScreen);

    Snapshot.PreviousScreen = Snapshot.CurrentScreen;
    Snapshot.CurrentScreen = Screen;
    Snapshot.WindowMode = Mode;
    OnScreenChanged.Broadcast(Screen);
    OnStateChanged.Broadcast();
    return true;
}

bool UAetherUISubsystem::CloseScreen()
{
    if (Snapshot.CurrentScreen == EAetherUIScreen::None) return false;
    if (Snapshot.bModalOpen) return false;
    return NavigateBack();
}

bool UAetherUISubsystem::NavigateBack()
{
    if (Snapshot.bModalOpen) return false;
    if (History.Num() == 0)
    {
        Snapshot.PreviousScreen = Snapshot.CurrentScreen;
        Snapshot.CurrentScreen = EAetherUIScreen::None;
        Snapshot.WindowMode = EAetherUIWindowMode::Fullscreen;
        OnScreenChanged.Broadcast(EAetherUIScreen::None);
        OnStateChanged.Broadcast();
        return true;
    }

    const EAetherUIScreen Next = History.Pop();
    Snapshot.PreviousScreen = Snapshot.CurrentScreen;
    Snapshot.CurrentScreen = Next;
    Snapshot.WindowMode = EAetherUIWindowMode::Fullscreen;
    OnScreenChanged.Broadcast(Next);
    OnStateChanged.Broadcast();
    return true;
}

bool UAetherUISubsystem::OpenModal(const FString& ModalID)
{
    const FString CleanID = ModalID.TrimStartAndEnd();
    if (CleanID.IsEmpty() || Snapshot.bModalOpen) return false;
    Snapshot.bModalOpen = true;
    Snapshot.ModalID = CleanID;
    Snapshot.WindowMode = EAetherUIWindowMode::Modal;
    OnStateChanged.Broadcast();
    return true;
}

void UAetherUISubsystem::CloseModal()
{
    if (!Snapshot.bModalOpen) return;
    Snapshot.bModalOpen = false;
    Snapshot.ModalID.Reset();
    Snapshot.WindowMode = EAetherUIWindowMode::Overlay;
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetClassSelection(const TArray<FAetherUIClassEntry>& Entries)
{
    Snapshot.ClassSelection = Entries;
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetInventory(const TArray<FAetherUIItemEntry>& Entries, int32 Capacity)
{
    Snapshot.Inventory = Entries;
    Snapshot.InventoryCapacity = FMath::Max(0, Capacity);
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetEquipment(const TArray<FAetherUIEquipmentEntry>& Entries)
{
    Snapshot.Equipment = Entries;
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetSkills(const TArray<FAetherUISkillEntry>& Entries)
{
    Snapshot.Skills = Entries;
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetQuests(const TArray<FAetherUIQuestEntry>& Entries)
{
    Snapshot.Quests = Entries;
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetSocial(const TArray<FAetherUISocialEntry>& Entries)
{
    Snapshot.Social = Entries;
    OnStateChanged.Broadcast();
}

void UAetherUISubsystem::SetSettings(const FAetherUISettings& Settings)
{
    if (!Settings.IsValid()) return;
    Snapshot.Settings = Settings;
    OnStateChanged.Broadcast();
}

uint32 UAetherUISubsystem::PushNotification(EAetherUINotificationType Type, const FString& Title, const FString& Message, double NowSeconds, double DurationSeconds)
{
    FAetherUINotification Notification;
    Notification.NotificationID = NextNotificationID++;
    if (NextNotificationID == 0) NextNotificationID = 1;
    Notification.Type = Type;
    Notification.Title = Title.TrimStartAndEnd();
    Notification.Message = Message.TrimStartAndEnd();
    Notification.CreatedAtSeconds = NowSeconds;
    Notification.DurationSeconds = FMath::Max(0.0, DurationSeconds);
    Snapshot.Notifications.Add(Notification);
    OnNotification.Broadcast(Notification);
    OnStateChanged.Broadcast();
    return Notification.NotificationID;
}

int32 UAetherUISubsystem::ExpireNotifications(double NowSeconds)
{
    int32 Removed = 0;
    for (int32 Index = Snapshot.Notifications.Num() - 1; Index >= 0; --Index)
    {
        if (Snapshot.Notifications[Index].IsExpired(NowSeconds))
        {
            Snapshot.Notifications.RemoveAt(Index);
            ++Removed;
        }
    }
    if (Removed > 0) OnStateChanged.Broadcast();
    return Removed;
}
