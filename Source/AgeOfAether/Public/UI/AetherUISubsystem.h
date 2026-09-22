#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/AetherUITypes.h"
#include "AetherUISubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherUIScreenChangedEvent, EAetherUIScreen, Screen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAetherUIStateChangedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherUINotificationEvent, const FAetherUINotification&, Notification);

UCLASS()
class AGEOFAETHER_API UAetherUISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void ResetUI();

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    bool OpenScreen(EAetherUIScreen Screen, EAetherUIWindowMode Mode = EAetherUIWindowMode::Fullscreen);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    bool CloseScreen();

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    bool NavigateBack();

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    bool OpenModal(const FString& ModalID);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void CloseModal();

    UFUNCTION(BlueprintPure, Category="Age of Aether|UI")
    bool IsModalOpen() const { return Snapshot.bModalOpen; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|UI")
    EAetherUIScreen GetCurrentScreen() const { return Snapshot.CurrentScreen; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|UI")
    FAetherUIRuntimeSnapshot GetSnapshot() const { return Snapshot; }

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetClassSelection(const TArray<FAetherUIClassEntry>& Entries);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetInventory(const TArray<FAetherUIItemEntry>& Entries, int32 Capacity);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetEquipment(const TArray<FAetherUIEquipmentEntry>& Entries);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetSkills(const TArray<FAetherUISkillEntry>& Entries);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetQuests(const TArray<FAetherUIQuestEntry>& Entries);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetSocial(const TArray<FAetherUISocialEntry>& Entries);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    void SetSettings(const FAetherUISettings& Settings);

    uint32 PushNotification(EAetherUINotificationType Type, const FString& Title, const FString& Message, double NowSeconds, double DurationSeconds = 5.0);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|UI")
    int32 ExpireNotifications(double NowSeconds);

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|UI")
    FAetherUIScreenChangedEvent OnScreenChanged;

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|UI")
    FAetherUIStateChangedEvent OnStateChanged;

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|UI")
    FAetherUINotificationEvent OnNotification;

private:
    FAetherUIRuntimeSnapshot Snapshot;
    TArray<EAetherUIScreen> History;
    uint32 NextNotificationID = 1;

    bool IsNavigable(EAetherUIScreen Screen) const;
};
